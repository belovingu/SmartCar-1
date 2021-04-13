
#include "distinguish.h"
#include "findLines.h"
#include "chassisThread.h"
#include "rtthread.h"

extern ebled_t info_beep;

int16 g_highest_speed = 0;
int16 g_lowest_speed = 0;
int16 speed_set = 0;
int16 speed_avg = 0;

int16 s_zhi = 200;
int16 s_max = 200;
int16 s_wan1 = 170;
int16 s_wan2 = 150;
int16 s_cross = 200;
int16 s_huan = 100;

int16 fore_min = 55;
int16 fore_max = 69;
int16 foresight = 69;

int16 mid_error[5];
int16 steer_error[30] = {0};
int16 steer_error_ave = 0;
int16 steer_error_l = 0;
int16 steer_error_ll = 0;

int16 stop_car[20] = {0};
int16 stop_car_flag = 0;
int16 road_type[200] = {0};

int16 shortstraight_flag;
int16 crossstraight_flag;

int16 long_straight_flag = 0;
int16 stdDeviation = 0;

#define long_straight_endrow 35
#define LONG_STRAIGHT 1
#define SHORT_STRAIGHT 2
#define CURVE 3
#define LOOP 4
#define CROSS 5
#define RAMP 6
#define BUMPY 7
#define OBSTACLE 8
//�������ͼ�¼
// 1Ϊ��ֱ����
// 2Ϊ��ֱ����
// 3Ϊ�䣬
// 4Ϊ����
// 5Ϊʮ�֣�
// 6Ϊ�µ���
// 7Ϊ������
// 8Ϊ�ϰ�

float mid_line_weight[img_height] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //1-40

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //41-60

    1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3,
    1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, //61-80

    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, //81-100

    2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //101-120
};

float solve_midline_slope(void)
{
    //��100-30�з�Χ�����߱�׼��
    int16 i, Sum1 = 0;
    float S = 0, Av = 0, Sum2 = 0;
    for (i = 100; i > 30; i--)
    {
        Sum1 += f1.midline[i];
    }
    Av = Sum1 / 70.0;
    for (i = 100; i > 30; i--)
    {
        Sum2 += (f1.midline[i] - Av) * (f1.midline[i] - Av);
    }
    S = sqrt(Sum2 / 70.0);
    return S;
}

void stop_car_judge()
{
    int16 i = 0, j = 0, count_black = 0;
    for (i = 118; i > 115; i--)
    {
        for (j = 130; j >= 30; j--)
        {
            if (Image[i][j] == 0)
            {
                count_black = count_black + 1;
            }
        }
    }
    if (count_black > 150)
    {
        stop_car[0] = 1;
    }
    else
    {
        stop_car[0] = 0;
    }

    int stop_count = 0;
    stop_car_flag = 0;
    for (int i = 20; i >= 1; i--)
    {
        stop_car[i] = stop_car[i - 1];
        if (stop_car[i])
        {
            stop_count++;
        }
    }
    if (stop_count >= 15)
    {
        stop_car_flag = 1;
    }
}

void long_straight_judge(void)
{ //��zhidao_endrow����ͷ��ξ����� 1���߲�̫ƫ 2Ҳ������ 3�Ҳ�Ϊʮ�ֺ��µ� 4��������ֱ������
    int16 lose_cnt = 0;
    int16 i = 0, flag1 = 0, flag2 = 0;
    flag1 = 1; //�����ߵ�Ҫ��
    flag2 = 1; //�Զ��ߵ�Ҫ��
    for (i = 118; i > long_straight_endrow; i--)
    {
        if (f1.midline[i] > 88 || f1.midline[i] < 72)
        {
            flag1 = 0;
            break;
        }
    }
    if (flag1)
    { //���߲��ܳ���3
        for (i = 100; i > long_straight_endrow; i--)
        {
            if (f1.leftlineflag[i] == 0 || f1.rightlineflag[i] == 0)
            {
                lose_cnt++;
            }
        }
        if (lose_cnt > 3)
        {
            flag2 = 0;
        }
    }
    if (f2.toppoint <= 37 && flag1 == 1 && flag2 == 1)
    {
        long_straight_flag = 1;
    }
}

void short_straight_judge(void)
{
    stdDeviation = (int16)solve_midline_slope();
    if (stdDeviation <= 6)
    {
        shortstraight_flag = 1;
    }
    else if (stdDeviation <= 11)
    {
        shortstraight_flag = 2;
    }
}

void cross_straight_judge(void)
{
    stdDeviation = (int16)solve_midline_slope();
    if (stdDeviation < 4)
    {
        crossstraight_flag = 1;
    }
    else if (stdDeviation <= 6)
    {
        crossstraight_flag = 2;
    }
}

void compute_error()
{
    speed_avg = (int16)(0.6 * speed_now[0] + 0.2 * speed_now[1] + 0.2 * speed_now[2]);
    // //��������ֵ���и�Ƶ����
    // //��õ�ʵ���ٶ�,���˲�

    // if (speed_avg > 370)
    // {
    //     foresight = fore_min;
    // }

    // else if (speed_avg < 250)
    // {
    //     foresight = fore_max;
    // }
    // else
    // {
    //     foresight = (int16)(fore_min + (float)(fore_max - fore_min) * (380 - speed_avg) * (380 - speed_avg) / (120 * 120));
    // }

    // foresight = limit_ab_int(foresight, fore_min, fore_max);

    // mid_error[0] = 1 * (f1.midline[foresight] - 80);
    // mid_error[1] = 1 * (f1.midline[foresight + 1] - 80);
    // mid_error[2] = 1 * (f1.midline[foresight + 2] - 80);
    // mid_error[3] = 1 * (f1.midline[foresight + 3] - 80);
    // mid_error[4] = 1 * (f1.midline[foresight + 4] - 80);
    // steer_error[0] = (int16)((mid_error[0] + mid_error[1] + mid_error[2] + mid_error[3] + mid_error[4]) / 5);
    float CenterSum = 0;
    float WeightSum = 0;
    float CenterMeanValue = 0;
    for (int i = 120 - 1; i > f2.toppoint; i--)
    {
        CenterSum += f1.midline[i] * mid_line_weight[i];
        WeightSum += mid_line_weight[i];
    }

    if (WeightSum != 0)
    {
        CenterMeanValue = (CenterSum / WeightSum);
    }
    steer_error[0] = (CenterMeanValue - 80);

    if (steer_error[0] > 80)
    {
        steer_error[0] = 80;
    }
    if (steer_error[0] < -80)
    {
        steer_error[0] = -80;
    }
    if (steer_error[0] - steer_error[1] > 15)
    {
        steer_error[0] = steer_error[1] + 15;
    }
    if (steer_error[0] - steer_error[1] < -15)
    {
        steer_error[0] = steer_error[1] - 15;
    }

    for (int i = 29; i > 0; i--)
    {
        steer_error[i] = steer_error[i - 1];
    }

    steer_error_ave = steer_error[0];
    // steer_error_ave = (int16)(0.6 * steer_error[0] + 0.2 * steer_error_l + 0.2 * steer_error_ll);
    steer_error_ll = steer_error_l;
    steer_error_l = steer_error_ave;
}

void velocity_set()
{

    long_straight_flag = 0;

    if (f2.shiziflag[1] == 0)
    {
        long_straight_judge();
        short_straight_judge();
    }

    if (f2.huanleftflag != 0 || f2.huanrightflag != 0 || f2.huan == 1)
    {
        road_type[0] = LOOP;
        if (road_type[1] != LOOP)
            easyblink(info_beep, 1, 50, 100);

        speed_set = s_huan;
    }
    else if (f2.shiziflag[1] == 1)
    {
        road_type[0] = CROSS;
        if (road_type[1] != CROSS)
            easyblink(info_beep, 2, 50, 100);

        speed_set = s_cross;
    }

    else if (long_straight_flag == 1)
    {
        road_type[0] = LONG_STRAIGHT;
        // if(road_type[40]==3&&road_type[41]==3&&road_type[42]==3&&road_type[43]==3&&road_type[44]==3)
        //     speed_set=s_max+100;//������� δ���� �������ע��
        // else
        speed_set = s_max;
    }

    else if (shortstraight_flag != 0)
    {
        road_type[0] = SHORT_STRAIGHT;
        if (road_type[6] == 1 && road_type[7] == 1 && road_type[8] == 1 && road_type[9] == 1 && road_type[10] == 1)
        { //ǰ8֡�ǳ���ֱ��,�ȼ���һ��
            speed_set = s_zhi - 70;
        }
        else if (road_type[3] == 3 && road_type[4] == 3 && road_type[5] == 3)
        { //ǰ3֡����� ����ŷķ���ڼ���
            speed_set = s_wan1;
        }
        else
        {
            speed_set = (int16)(s_zhi - (s_zhi - s_wan1) * (abs(steer_error_ave) / 35)); //ƫ�����35������ٶ�
            speed_set = speed_set < s_wan1 ? s_wan1 : speed_set;
        }
    }

    else if (f2.huanleftflag != 0 || f2.huanrightflag != 0 || f2.huan == 1)
    {
        road_type[0] = CROSS;
        speed_set = s_huan;
    }
    else
    {
        road_type[0] = CURVE;
        if (road_type[9] == 1 && road_type[10] == 1 && road_type[11] == 1 && road_type[12] == 1 && road_type[13] == 1)
        {
            speed_set = s_wan1 - 30;
        }
        else
        {
            g_highest_speed = s_wan1;
            g_lowest_speed = s_wan2;
            speed_set = g_highest_speed - (g_highest_speed - g_lowest_speed) * (steer_error_ave * steer_error_ave) / (40 * 40); //ƫ�����40������ٶ�
            speed_set = speed_set < g_lowest_speed ? g_lowest_speed : speed_set;
            speed_set = speed_set > g_highest_speed ? g_highest_speed : speed_set; //��Сs_wan2���s_wan1
        }
    }

    for (int i = 200; i >= 1; i--)
    {
        road_type[i] = road_type[i - 1];
    }
}

void dym_param_set_cmd(int argc, char **argv)
{
    s_zhi = atoi(argv[1]);
    s_max = atoi(argv[2]);
    s_wan1 = atoi(argv[3]);
    s_wan2 = atoi(argv[4]);
    s_cross = atoi(argv[5]);
    s_huan = atoi(argv[6]);

    fore_min = atoi(argv[7]);
    fore_max = atoi(argv[8]);
}
MSH_CMD_EXPORT(dym_param_set_cmd, dym_param_set_cmd);