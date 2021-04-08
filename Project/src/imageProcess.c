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

//left right line
int left_line[img_height];
int right_line[img_height];
int mid_line[img_height];
int mid_line_fit3[img_height];
int road_width[img_height];

int left_line_lose_flag[img_height];
int right_line_lose_flag[img_height];
int both_line_lose_flag[img_height];
int mid_line_lose_flag[img_height];

int left_line_lose_num = 0;
int right_line_lose_num = 0;
int both_line_lose_num = 0;

int line_end_pos_max = 0;
int cross_road_flag = 0;

//road end
int road_end_pos = 0;
uint8_t road_stop_flag = 0;
int available_line_num = 0;

//line end pos
int midright_line_end_pos = 0;
int midleft_line_end_pos = 0;
int leftleft_line_end_pos = 0;
int rightright_line_end_pos = 0;

int left_line_end_pos = 0;
int mid_line_end_pos = 0;
int right_line_end_pos = 0;

//loop detect
int RightInflectionPointRow = 0;
int RightInflectionPointCol = 0;
int RightInflectionPointFlag = 0;

int LeftInflectionPointRow = 0;
int LeftInflectionPointCol = 0;
int LeftInflectionPointFlag = 0;

uint8_t loop_detected_flag = 0;

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

// void DiffSumFindLine()
// {
// }

void LeastFit3(double *x, double *y, int num,
               double *a, double *b, double *c, double *d)
{
    /**
     * 说明：3阶拟合函数
     * x：x坐标值
     * y：y坐标值
     * num：数据个数
     * a,b,c,d：返回拟合系数y = ax3 + bx2 + cx + d
     */
    double sum_x = 0;
    double sum_y = 0;
    double sum_x2 = 0;
    double sum_x3 = 0;
    double sum_x4 = 0;
    double sum_x5 = 0;
    double sum_x6 = 0;
    double sum_xy = 0;
    double sum_x2y = 0;
    double sum_x3y = 0;

    for (int i = 0; i < num; ++i)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_x2 += x[i] * x[i];
        sum_x3 += x[i] * x[i] * x[i];
        sum_x4 += x[i] * x[i] * x[i] * x[i];
        sum_x5 += x[i] * x[i] * x[i] * x[i] * x[i];
        sum_x6 += x[i] * x[i] * x[i] * x[i] * x[i] * x[i];
        sum_xy += x[i] * y[i];
        sum_x2y += x[i] * x[i] * y[i];
        sum_x3y += x[i] * x[i] * x[i] * y[i];
    }

    sum_x /= num;
    sum_y /= num;
    sum_x2 /= num;
    sum_x3 /= num;
    sum_x4 /= num;
    sum_x5 /= num;
    sum_x6 /= num;
    sum_xy /= num;
    sum_x2y /= num;
    sum_x3y /= num;

    double m1 = (sum_x3 * sum_x6 - sum_x4 * sum_x5) / (sum_x4 * sum_x6 - sum_x5 * sum_x5);
    double n1 = (sum_x2 * sum_x6 - sum_x3 * sum_x5) / (sum_x4 * sum_x6 - sum_x5 * sum_x5);
    double k1 = (sum_x2y * sum_x6 - sum_x3y * sum_x5) / (sum_x4 * sum_x6 - sum_x5 * sum_x5);

    double m2 = (sum_x2 * sum_x6 - sum_x4 * sum_x4) / (sum_x3 * sum_x6 - sum_x5 * sum_x4);
    double n2 = (sum_x * sum_x6 - sum_x3 * sum_x4) / (sum_x3 * sum_x6 - sum_x5 * sum_x4);
    double k2 = (sum_xy * sum_x6 - sum_x3y * sum_x4) / (sum_x3 * sum_x6 - sum_x5 * sum_x4);

    double m3 = (sum_x * sum_x6 - sum_x4 * sum_x3) / (sum_x2 * sum_x6 - sum_x5 * sum_x3);
    double n3 = (sum_x6 - sum_x3 * sum_x3) / (sum_x2 * sum_x6 - sum_x5 * sum_x3);
    double k3 = (sum_y * sum_x6 - sum_x3y * sum_x3) / (sum_x2 * sum_x6 - sum_x5 * sum_x3);

    *d = ((k3 - k1) / (m3 - m1) - (k2 - k1) / (m2 - m1)) / ((n3 - n1) / (m3 - m1) - (n2 - n1) / (m2 - m1));
    *c = (k2 - k1) / (m2 - m1) - (n2 - n1) / (m2 - m1) * (*d);
    *b = k1 - m1 * (*c) - n1 * (*d);
    *a = sum_x3y / sum_x6 - sum_x5 / sum_x6 * (*b) - sum_x4 / sum_x6 * (*c) - sum_x3 / sum_x6 * (*d);
}

void Least3Error()
{
    double a, b, c, d;
    double data_x[img_height];
    double data_y[img_width];
    int i = 0, j = 0, k = 0;

    for (i = road_end_pos; i < img_height; i++)
    {
        data_y[j] = mid_line[i];
        data_x[j] = i;
        j++;
    }
    k = img_height - road_end_pos;
    LeastFit3(data_x, data_y, k, &a, &b, &c, &d);
    for (i = road_end_pos; i < img_height; i++)
    {
        mid_line_fit3[i] = (int)(a * pow(i, 3) + b * pow(i, 2) + c * i + d);
    }
}

void ResetVarialble()
{
    left_line_lose_num = 0;
    right_line_lose_num = 0;
    both_line_lose_num = 0;
    cross_road_flag = 0;
    road_end_pos = 0;
    available_line_num = 0;
    road_stop_flag = 0;
    for (int i = 0; i < img_height; i++)
    {
        left_line[i] = 0;
        right_line[i] = img_width - 1;
        road_width[i] = img_width - 1;
        mid_line[i] = img_width / 2;
        mid_line_fit3[i] = img_width / 2;

        left_line_lose_flag[i] = 0;
        right_line_lose_flag[i] = 0;
        both_line_lose_flag[i] = 0;
        mid_line_lose_flag[i] = 0;
    }
}

//first line all white
//return 1: stop, 0: run
int StopCarJudge(const uint8_t img[img_height][img_width])
{
    for (int i = 0; i < img_width; i++)
    {
        if (img[img_height - 1][i] == white_point)
        {
            return 0;
        }
    }
    return 1;
}

void CrossRoadProcess(const uint8_t img[img_height][img_width])
{
    uint8_t left_line_end_flag = 0; //标志位
    uint8_t mid_line_end_flag = 0;
    uint8_t right_line_end_flag = 0;
    uint8_t MREndFlag = 0;
    uint8_t MLEndFlag = 0;
    uint8_t LLEndFlag = 0;
    uint8_t RREndFlag = 0;

    midright_line_end_pos = 0; //清零
    midleft_line_end_pos = 0;
    leftleft_line_end_pos = 0;
    rightright_line_end_pos = 0;
    left_line_end_pos = 0;
    mid_line_end_pos = 0;
    right_line_end_pos = 0;

    for (int i = img_height - 1; i >= 3; i--)
    {
        if (img[i][img_width / 2] == white_point &&
            !mid_line_end_flag) //!MEndFlag=1 //40
        {
            mid_line_end_pos++; //中黑线截至行
        }
        else if (i > 1 && img[i - 1][img_width / 2] == black_point &&
                 img[i - 2][img_width / 2] == black_point) //连续两行是黑色
        {
            mid_line_end_flag = 1;
        }

        if (img[i][img_width / 4] == white_point &&
            !left_line_end_flag) //20
        {
            left_line_end_pos++; //左黑线截至行
        }
        else if (i > 1 && img[i - 1][img_width / 4] == black_point &&
                 img[i - 2][img_width / 4] == black_point)
        {
            left_line_end_flag = 1;
        }

        if (img[i][img_width * 3 / 4] == white_point &&
            !right_line_end_flag) //60
        {
            right_line_end_pos++; //右黑线截至行
        }
        else if (i > 1 && img[i - 1][img_width * 3 / 4] == black_point &&
                 img[i - 2][img_width * 3 / 4] == black_point)
        {
            right_line_end_flag = 1;
        }
        //mid left
        if (img[i][img_width * 3 / 8] == white_point &&
            !MLEndFlag)
        {
            midleft_line_end_pos++;
        }
        else if (i > 1 && img[i - 1][img_width * 3 / 8] == black_point &&
                 img[i - 2][img_width * 3 / 8] == black_point)
        {
            MLEndFlag = 1;
        }
        //mid_right
        if (img[i][img_width * 5 / 8] == white_point &&
            !MREndFlag)
        {
            midright_line_end_pos++;
        }
        else if (i > 1 && img[i - 1][img_width * 5 / 8] == black_point &&
                 img[i - 2][img_width * 5 / 8] == black_point)
        {
            MREndFlag = 1;
        }
        //most left
        if (img[i][img_width * 1 / 8] == white_point &&
            !LLEndFlag)
        {
            leftleft_line_end_pos++;
        }
        else if (i > 1 && img[i - 1][img_width * 1 / 8] == black_point &&
                 img[i - 2][img_width * 1 / 8] == black_point)
        {
            LLEndFlag = 1;
        }
        //most right
        if (img[i][img_width * 7 / 8] == white_point &&
            !RREndFlag)
        {
            rightright_line_end_pos++;
        }
        else if (i > 1 && img[i - 1][img_width * 7 / 8] == black_point &&
                 img[i - 2][img_width * 7 / 8] == black_point)
        {
            RREndFlag = 1;
        }
    }

    line_end_pos_max = MAX(left_line_end_pos, mid_line_end_pos);
    line_end_pos_max = MAX(line_end_pos_max, right_line_end_pos);
    line_end_pos_max = MAX(line_end_pos_max, midright_line_end_pos);
    line_end_pos_max = MAX(line_end_pos_max, midleft_line_end_pos);
    line_end_pos_max = MAX(line_end_pos_max, leftleft_line_end_pos);
    line_end_pos_max = MAX(line_end_pos_max, rightright_line_end_pos);

    //CrossRoadTrick
    if (both_line_lose_num > img_height / 5)
    {
        cross_road_flag = 1;
        if ((mid_line_end_pos >= left_line_end_pos) &&
            (mid_line_end_pos >= right_line_end_pos)) //中间白线最多
        {
            for (int i = 0; i < img_height; i++)
            {
                if ((both_line_lose_flag[i] == 1) && (mid_line_lose_flag[i] != 1))
                {
                    mid_line[i] = img_width / 2;
                }
            }
        }
        else if ((left_line_end_pos > mid_line_end_pos) &&
                 (left_line_end_pos > right_line_end_pos)) //看到左边白的最多，车体偏右了，往左跑
        {
            for (int i = 0; i < img_height; i++)
            {
                if ((both_line_lose_flag[i] == 1) && (mid_line_lose_flag[i] != 1))
                {
                    mid_line[i] = img_width * 7 / 16;
                }
            }
        }
        else if ((right_line_end_pos > mid_line_end_pos) &&
                 (right_line_end_pos > left_line_end_pos)) //看到右边白的最多，车体偏左了，往右跑
        {
            for (int i = 0; i < img_height; i++)
            {
                if ((both_line_lose_flag[i] == 1) && (mid_line_lose_flag[i] != 1))
                {
                    mid_line[i] = img_width * 9 / 16;
                }
            }
        }
    }
}

//call after findmidline
void LoopRoadDetect()
{
    int i;
    static int loop_detect_state = 0;
    loop_detected_flag = 0;

    switch (loop_detect_state)
    {
    case 0:
        //right loop enter
        for (i = img_height - 5; i >= img_height * 6 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !right_line_lose_flag[i])
            {
                if ((right_line[i - 2] - right_line[i]) > 20)
                {

                    loop_detect_state += 1;
                    break;
                }
            }
        }
        break;
    case 1:
        //right loop
        for (i = img_height * 2 / 12; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !left_line_lose_flag[i - 4] &&
                right_line_lose_flag[i] &&
                right_line_lose_flag[i - 2] &&
                right_line_lose_flag[i - 4])
            {
                loop_detect_state += 1;
                break;
            }
        }
        break;
    case 2:
        //right loop
        for (i = img_height - 5; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !left_line_lose_flag[i + 2] &&
                !right_line_lose_flag[i] &&
                !right_line_lose_flag[i - 2] &&
                !right_line_lose_flag[i + 2])
            {
                if ((right_line[i] - right_line[i + 2] <= 0) &&
                    (right_line[i - 2] - right_line[i]) > 0)
                {

                    // RightInflectionPointRow = i;
                    // RightInflectionPointCol = right_line[i];
                    // RightInflectionPointFlag = 1;
                    loop_detect_state += 1;
                    break;
                }
            }
        }
        break;
    case 3:
        //right loop
        for (i = img_height * 2 / 12; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !left_line_lose_flag[i - 4] &&
                right_line_lose_flag[i] &&
                right_line_lose_flag[i - 2] &&
                right_line_lose_flag[i - 4])
            {
                loop_detect_state += 1;
                break;
            }
        }
        break;
    case 4:
        for (i = img_height - 5; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !right_line_lose_flag[i])
            {
                if ((right_line[i] - right_line[i - 2]) > 20)
                {
                    loop_detect_state = 0;
                    loop_detected_flag = 1;
                    break;
                }
            }
        }
        break;
    default:
        loop_detect_state = 0;
        break;
    }
}

void LoopRoadProcess()
{
    int point1_x = 0, point1_y = 0;
    int point2_x = 0, point2_y = 0;
    static int loop_process_state = 0;
    int stop_loop_line_fix_flag1 = 1, stop_loop_line_fix_flag2 = 1;
    float step;

    switch (loop_process_state)
    {
    case 0:
        LoopRoadDetect();
        if (loop_detected_flag)
        {
            loop_process_state += 1;
        }
        break;
    case 1:
        //point1
        for (int i = img_height - 5; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !right_line_lose_flag[i])
            {
                if ((right_line[i] - right_line[i - 2]) > 20)
                {
                    point1_x = right_line[i];
                    point1_y = i;
                    stop_loop_line_fix_flag1 = 0;
                    break;
                }
            }
        }
        //point2
        for (int i = img_height - 5; i >= img_height * 9 / 12; i--)
        {
            if (!left_line_lose_flag[i] &&
                !left_line_lose_flag[i - 2] &&
                !right_line_lose_flag[i] &&
                !right_line_lose_flag[i - 2])
            {
                if ((right_line[i - 2] - right_line[i]) > 4)
                {
                    point2_x = left_line[i];
                    point2_y = i;
                    stop_loop_line_fix_flag2 = 0;
                    break;
                }
            }
        }
        //stop fix
        if (stop_loop_line_fix_flag1 || stop_loop_line_fix_flag2)
        {
            loop_process_state += 1;
            break;
        }
        // ASSERT(point2_y > point1_y);
        step = (point2_x - point1_x) / (point2_y - point1_y);
        for (int i = point1_y; i < point2_y; i++)
        {
            left_line[i] = (int)(point1_x - step * (i - point1_y));
        }
        break;

    case 2:
        break;
    default:
        loop_process_state = 0;
        break;
    }
}

void FindEdgeLine(const uint8_t img[img_height][img_width])
{
    int i, j;

    for (i = img_height - 1; i >= 0; i--)
    {
        //left line
        if (i == img_height - 1)
        {
            j = img_width / 2;
        }
        else
        {
            j = mid_line[i + 1];
        }
        if (j <= 2)
        {
            j = 2;
        }
        while (j >= 3)
        {
            if (img[i][j] == white_point &&
                img[i][j - 1] == black_point &&
                img[i][j - 2] == black_point)
            {
                left_line[i] = j;
                break;
            }
            j--;
        }

        //right line
        if (i == img_height - 1)
        {
            j = img_width / 2;
        }
        else
        {
            j = mid_line[i + 1];
        }
        if (j >= img_width - 3)
        {
            j = img_width - 3;
        }
        while (j <= img_width - 3)
        {
            if (img[i][j] == white_point &&
                img[i][j + 1] == black_point &&
                img[i][j + 2] == black_point)
            {
                right_line[i] = j;
                break;
            }
            j++;
        }
        road_width[i] = right_line[i] - left_line[i];
    }
}

void FindMidLine(const uint8_t img[img_height][img_width])
{
    int i;
    for (i = img_height - 1; i >= 0; i--)
    {
        //mid_line
        if (left_line[i] != 0 && right_line[i] != img_width - 1)
        {
            mid_line[i] = (left_line[i] + right_line[i]) / 2;
        }
        else if (left_line[i] != 0 && right_line[i] == img_width - 1)
        {
            right_line_lose_flag[i] = 1;
            right_line_lose_num++;

            if ((right_line[i] - left_line[i]) >= (right_line[i + 1] - left_line[i + 1] + 1)) //突变
            {
                mid_line[i] = mid_line[i + 1]; //用上一行的中点
            }
            else
            {
                mid_line[i] = left_line[i] + road_width[i] / 2; //正常的话就用半宽补
            }
        }
        else if (left_line[i] == 0 && right_line[i] != img_width - 1) //丢了左线
        {
            left_line_lose_flag[i] = 1;
            left_line_lose_num++;

            if ((right_line[i] - left_line[i]) >= (right_line[i + 1] - left_line[i + 1] + 1)) //突变
            {
                mid_line[i] = mid_line[i + 1]; //用上一行
            }
            else
            {
                mid_line[i] = right_line[i] - road_width[i] / 2; //线宽
            }
        }
        else if (left_line[i] == 0 && right_line[i] == img_width - 1) //两边都丢了的话
        {

            both_line_lose_flag[i] = 1;
            both_line_lose_num++;

            if (i == img_height - 1) //如果是首行就以图像中心作为中点
            {
                mid_line[i] = img_width / 2;
            }
            else
            {
                mid_line[i] = mid_line[i + 1]; //如果不是首行就用上一行的中线作为本行中点
            }
        }

        //lose mid line
        if (img[i][mid_line[i]] == black_point)
        {
            mid_line_lose_flag[i] = 1;
            mid_line[i] = img_width / 2;
        }

        mid_line[i] = limit_ab_int(mid_line[i], 0, img_width - 1);

        //road end
        if (!road_stop_flag && ((left_line[i] != 0 && left_line[i] >= img_width * 0.8) ||
                                (right_line[i] != img_width - 1 && right_line[i] < img_width * 0.2) ||
                                (i >= 1) && (img[i][mid_line[i]] == black_point))) //最后一行
        {
            road_end_pos = i;
            available_line_num = img_height - i;
            road_stop_flag = 1;
        }
    }
}

void MeanFilter(const uint8_t img[img_height][img_width],
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

void Convert2Binary(const uint8_t img[img_height][img_width],
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
