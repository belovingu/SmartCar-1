#include "headfile.h"

#define BUFFER_SIZE (512U)

static FATFS g_fileSystem;
static FIL g_fileObject;

AT_DTCM_SECTION_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)], MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
AT_DTCM_SECTION_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)], MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));

int test_file(void)
{
    FRESULT error;
    DIR directory; //目录结构体变量
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    volatile bool failedFlag = false;
    char ch = '0';
    BYTE work[FF_MAX_SS];

    DisableGlobalIRQ();
    board_init(); //务必保留，本函数用于初始化MPU 时钟 调试串口

    sdcard_clk_init(); //初始化SDHC时钟

    //总中断最后开启
    EnableGlobalIRQ(0);

    PRINTF("\r\nFATFS 文件系统示例程序\r\n");

    PRINTF("\r\n请插入SD卡\r\n");

    //等待SD卡插入
    if (sdcard_wait_card_insert() != kStatus_Success)
    {
        return -1;
    }

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("挂载失败\r\n");
        return -1;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("改变驱动器失败\r\n");
        return -1;
    }
#endif

#if FF_USE_MKFS
    PRINTF("\r\n创建文件系统中。。。这可能需要一个比较长的时间\r\n");
    //当SD卡文件系统创建完毕之后，就不需要在每次都创建了
    //因为每次创建都是会把SD卡格式化一遍的，每次都格式化容易导致卡报废
    if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
    {
        PRINTF("创建文件系统失败\r\n");
        return -1;
    }
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr); //创建文件系统后关闭SD卡电源
    SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);  //打开sd卡电源
#endif                                                  /* FF_USE_MKFS */

    PRINTF("\r\n创建目录中\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("目录已存在\r\n");
        }
        else
        {
            PRINTF("目录创建失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n创建一个文件在当前目录下\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("文件已存在\r\n");
        }
        else
        {
            PRINTF("创建文件失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n在当前目录中，再创建一个目录\r\n");
    error = f_mkdir(_T("/dir_1/dir_2"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("目录以存在\r\n");
        }
        else
        {
            PRINTF("目录创建失败\r\n");
            return -1;
        }
    }

    PRINTF("\r\n列出该目录中的文件\r\n");
    if (f_opendir(&directory, "/dir_1"))
    {
        PRINTF("目录打开失败\r\n");
        return -1;
    }

    for (;;)
    {
        error = f_readdir(&directory, &fileInformation);

        /* To the end. */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
        {
            break;
        }
        if (fileInformation.fname[0] == '.')
        {
            continue;
        }
        if (fileInformation.fattrib & AM_DIR)
        {
            PRINTF("文件夹 : %s.\r\n", fileInformation.fname);
        }
        else
        {
            PRINTF("普通文件 : %s.\r\n", fileInformation.fname);
        }
    }

    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';

    PRINTF("\r\n写入/读取文件，直到遇到错误\r\n");
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        PRINTF("\r\n写入数据到上面创建的文件\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("写入文件失败 \r\n");
            failedFlag = true;
            continue;
        }

        /* Move the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("设置文件指针位置失败\r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("读取上面创建的文件\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("读取文件失败\r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("开始比较写入与读取数据是否一致\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("写入与读取数据不一致\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("写入与读取数据一致\r\n");

        PRINTF("\r\n输入q退出程序 输入其他重复执行写入与读取\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\n示例程序不会再进行读取操作\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\n关闭文件失败\r\n");
        return -1;
    }

    while (true)
    {
    }
}
MSH_CMD_EXPORT(test_file, test_file);