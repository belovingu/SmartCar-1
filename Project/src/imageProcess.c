#include "headfile.h"
#include "imageProcess.h"
#include "util.h"
#include "math.h"

//constants
uint8_t ImageThreshold;
uint8_t HistoGram[256];

uint8_t BinImage[img_height][img_width];
uint8_t FilterImage[img_height][img_width];
uint8_t compressedImage[img_width * img_height / 8];

void gray_mean_filter(const uint8_t img[img_height][img_width],
                      uint8_t img_out[img_height][img_width])
{
    int count_x, count_y;

    /*3*3模版滤波计算，不计算边缘像素*/
    for (count_y = 1; count_y < img_height; count_y++)
    {
        for (count_x = 1; count_x < img_width; count_x++)
        {
            img_out[count_y][count_x] = (int)((img[count_y - 1][count_x - 1] +
                                               img[count_y - 1][count_x] +
                                               img[count_y - 1][count_x + 1] +
                                               img[count_y][count_x - 1] +
                                               img[count_y][count_x] +
                                               img[count_y][count_x + 1] +
                                               img[count_y + 1][count_x - 1] +
                                               img[count_y + 1][count_x] +
                                               img[count_y + 1][count_x + 1]) /
                                              9);
        }
    }

    /*对四个边缘直接进行赋值处理*/
    for (count_x = 0; count_x < img_width; count_x++) //水平边缘像素等于原来像素灰度值
    {
        img_out[0][count_x] = img[0][count_x];
        img_out[img_height - 1][count_x] = img[img_height - 1][count_x];
    }
    for (count_y = 1; count_y < img_height - 1; count_y++) //竖直边缘像素等于原来像素灰度值
    {
        img_out[count_y][0] = img[count_y][0];
        img_out[count_y][img_width - 1] = img[count_y][img_width - 1];
    }
}

void gray_to_binary(const uint8_t img[img_height][img_width],
                    uint8_t img_out[img_height][img_width],
                    uint8_t mode)
{
    //mode:  0：使用大津法阈值    1：使用平均阈值
    int i = 0, j = 0;

    if (mode)
    {
        ImageThreshold = LQMeanThreshold(img);
    }
    else
    {
        ImageThreshold = GetOSTUThreshold(img); //大津法阈值Threshold = (uint8_t)(Threshold * 0.5) + 70;
    }

    for (i = 0; i < img_height; i++)
    {
        for (j = 0; j < img_width; j++)
        {
            if (img[i][j] > ImageThreshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                img_out[i][j] = 255;
            else
                img_out[i][j] = 0;
        }
    }
}

int LQMeanThreshold(const uint8_t img[img_height][img_width])
{
    int i = 0, j = 0;
    uint32_t tv = 0;
    int threshold = 0;

    for (i = 0; i < img_width; i++)
    {
        for (j = 0; j < img_height; j++)
        {
            tv += img[j][i];
        }
    }
    threshold = tv / img_width / img_height; //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
    threshold = threshold * 7 / 10 + 10;     //此处阈值设置，根据环境的光线来设定
    return threshold;
}

int GetOSTUThreshold(const uint8_t img[img_height][img_width])
{
    int16_t i, j;
    uint32_t Amount = 0;
    uint32_t PixelBack = 0;
    uint32_t PixelIntegralBack = 0;
    uint32_t PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;
    // uint8_t HistoGram[256];

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < img_width; j++)
    {
        for (i = 0; i < img_height; i++)
        {
            HistoGram[img[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ; //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue; // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue; // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j]; //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
        PixelFore = Amount - PixelBack;                                                    //背景像素点数
        OmegaBack = (float)PixelBack / Amount;                                             //前景像素百分比
        OmegaFore = (float)PixelFore / Amount;                                             //背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;                                  //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;                                  //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
        if (Sigma > SigmaB)                                                                //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold; //返回最佳阈值;
}

void LQSobelFilter(const uint8_t imageIn[img_height][img_width],
                   uint8_t imageOut[img_height][img_width],
                   uint8_t Threshold)
{
    /** 卷积核大小 */
    short KERNEL_SIZE = 3;
    short xStart = KERNEL_SIZE / 2;
    short xEnd = img_width - KERNEL_SIZE / 2;
    short yStart = KERNEL_SIZE / 2;
    short yEnd = img_height - KERNEL_SIZE / 2;
    short i, j, k;
    short temp[4];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short)imageIn[i - 1][j - 1] + (short)imageIn[i - 1][j + 1]   //{{-1, 0, 1},
                      - (short)imageIn[i][j - 1] + (short)imageIn[i][j + 1]          // {-1, 0, 1},
                      - (short)imageIn[i + 1][j - 1] + (short)imageIn[i + 1][j + 1]; // {-1, 0, 1}};

            temp[1] = -(short)imageIn[i - 1][j - 1] + (short)imageIn[i + 1][j - 1]   //{{-1, -1, -1},
                      - (short)imageIn[i - 1][j] + (short)imageIn[i + 1][j]          // { 0,  0,  0},
                      - (short)imageIn[i - 1][j + 1] + (short)imageIn[i + 1][j + 1]; // { 1,  1,  1}};

            temp[2] = -(short)imageIn[i - 1][j] + (short)imageIn[i][j - 1]           //  0, -1, -1
                      - (short)imageIn[i][j + 1] + (short)imageIn[i + 1][j]          //  1,  0, -1
                      - (short)imageIn[i - 1][j + 1] + (short)imageIn[i + 1][j - 1]; //  1,  1,  0

            temp[3] = -(short)imageIn[i - 1][j] + (short)imageIn[i][j + 1]           // -1, -1,  0
                      - (short)imageIn[i][j - 1] + (short)imageIn[i + 1][j]          // -1,  0,  1
                      - (short)imageIn[i - 1][j - 1] + (short)imageIn[i + 1][j + 1]; //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for (k = 1; k < 4; k++)
            {
                if (temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if (temp[0] > Threshold)
            {
                imageOut[i][j] = 1;
            }
            else
            {
                imageOut[i][j] = 0;
            }
        }
    }
}
