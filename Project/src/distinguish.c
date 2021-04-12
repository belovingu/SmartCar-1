#include "findLines.h"
#include "distinguish.h"

int16 g_HighestSpeed = 0;
int16 g_LowestSpeed = 0;
int16 speed_set = 0;
int16 speed_now = 0;
int16 s_zhi = 250;
int16 s_max = 250;
int16 s_wan1 = 230;
int16 s_wan2 = 200;
int16 s_cross = 230;
int16 s_huan = 200;

int16 error_ave = 0;
int16 error_last = 0;
int16 error_pre = 0;
int16 fore_min = 55;
int16 fore_max = 69;
int16 foresight = 51;
int16 mid_error[5];
int16 error[30] = {0};
int16 error_cha[4] = {0};
int16 error_d = 0;

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

int16 tiaobian_cnt = 0;

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
    if (count_black > 200)
    {
        stop_car_flag = 1;
    }
    else
    {
        stop_car_flag = 0;
    }

    stop_car[0] = stop_car_flag;
    for (int i = 20; i >= 1; i--)
    {
        stop_car[i] = stop_car[i - 1];
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
    // speed_now = (int16)(0.6 * MotorPID.Speed_test[0] + 0.2 * MotorPID.Speed_test[1] + 0.2 * MotorPID.Speed_test[2]); //��������ֵ���и�Ƶ����

    // if (speed_now > 370)
    // {
    //     foresight = fore_min;
    // }

    // else if (speed_now < 250)
    // {
    //     foresight = fore_max;
    // }
    // else
    // {
    //     foresight = (int16)(fore_min + (float)(fore_max - fore_min) * (380 - speed_now) * (380 - speed_now) / (120 * 120));
    // }
    foresight = 55;
    foresight = foresight > 69 ? 69 : foresight;

    mid_error[0] = 8 * (f1.midline[foresight] - 80);
    mid_error[1] = 1 * (f1.midline[foresight + 1] - 80);
    mid_error[2] = 1 * (f1.midline[foresight + 2] - 80);
    error[0] = (int16)((mid_error[0] + mid_error[1] + mid_error[2]) / 10); //���������͵����ȳ˷���Ч�ʲ��

    if (error[0] > 60)
    { //error�޷� ����60
        error[0] = 60;
    }
    if (error[0] < -60)
    {
        error[0] = -60;
    }
    if (error[0] - error[1] > 15)
    { //error�仯�޷�
        error[0] = error[1] + 15;
    }
    if (error[0] - error[1] < -15)
    {
        error[0] = error[1] - 15;
    }

    error_cha[0] = error[0] - error[1];
    if (error_cha[0] - error_cha[1] > 13)
    { //error΢�ֵı仯�޷� ����ec0���40 ʵ��ʾ�������ز���15
        error_cha[0] = error_cha[1] + 13;
    }
    if (error_cha[0] - error_cha[1] < -13)
    {
        error_cha[0] = error_cha[1] - 13;
    }
    error_d = (int16)(0.8 * error_cha[1] + 0.2 * error_cha[0]); //2֡D

    error_cha[3] = error_cha[2];
    error_cha[2] = error_cha[1];
    error_cha[1] = error_cha[0];
    int i;
    for (i = 29; i > 0; i--)
    {
        error[i] = error[i - 1];
    }
}

void road_type_judge()
{
    error_ave = (int16)(0.6 * error[0] + 0.2 * error_last + 0.2 * error_pre);
    error_pre = error_last;
    error_last = error_ave;
    long_straight_flag = 0;

    if (f2.shiziflag[1] == 0)
    {
        long_straight_judge();
        short_straight_judge();
    }

    /**********�ٶȸ���*************/
    // #define LONG_STRAIGHT 1
    // #define SHORT_STRAIGHT 2
    // #define CURVE 3
    // #define LOOP 4
    // #define CROSS 5
    // #define RAMP 6
    // #define BUMPY 7
    // #define OBSTACLE 8
    if (f2.huanleftflag != 0 || f2.huanrightflag != 0 || f2.huan == 1)
    {
        road_type[0] = LOOP;
        speed_set = s_huan;
    }
    else if (f2.shiziflag[1] == 1)
    {
        road_type[0] = CROSS;
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
            speed_set = (int16)(s_zhi - (s_zhi - s_wan1) * (abs(error_ave) / 35)); //ƫ�����35������ٶ�
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
        { //ǰ6֡�ǳ���ֱ��,�ȼ���һ��
            speed_set = s_wan1 - 30;
        }
        else
        {
            g_HighestSpeed = s_wan1;
            g_LowestSpeed = s_wan2;
            //��̬��������ٶȺ�����ٶȣ���������ֱ����Ǳ��
            speed_set = g_HighestSpeed - (g_HighestSpeed - g_LowestSpeed) * (error_ave * error_ave) / (40 * 40); //ƫ�����40������ٶ�
            speed_set = speed_set < g_LowestSpeed ? g_LowestSpeed : speed_set;
            speed_set = speed_set > g_HighestSpeed ? g_HighestSpeed : speed_set; //��Сs_wan2���s_wan1
        }
    }

    for (int i = 200; i >= 1; i--)
    {
        road_type[i] = road_type[i - 1];
    }
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	  void sudu_set(void)
*��������:	  ��ȡ�ٶ�ģʽ	
���������        ��
���������        ��
ֻ�г�ֱ ֱ �µ� ʮ���ж�
*******************************************************************************/
// void sudu_set(void)
// {
//     int16 i = 0;
//     error_ave = (int16)(0.6 * error[0] + 0.2 * error_last + 0.2 * error_pre); //�˲�
//     error_pre = error_last;
//     error_last = error_ave;
//     super_zhidao = 0;

//     if (f2.shiziflag[1] == 0)
//     {
//         long_straight_judge();
//         short_straight_judge(); //ֱ���ж�
//     }

//     /**********�ٶȸ���*************/
//     if (f2.huanleftflag != 0 || f2.huanrightflag != 0 || f2.huan == 1)
//     {
//         road_type[0] = 4;
//         MotorPID.SpeedSet = s_huan;
//     }
//     else if (f2.shiziflag[1] == 1)
//     {
//         road_type[0] = 5;
//         LedOff(1);
//         LedOff(3);
//         LedOff(2);
//         LedOn(3);
//         LedOn(2); //��ɫ

//         MotorPID.SpeedSet = s_cross;
//     }

//     else if (super_zhidao == 1)
//     {
//         road_type[0] = 1;
//         LedOff(1);
//         LedOff(3);
//         LedOff(2);
//         LedOn(3); //��ɫ

//         if (road_type[40] == 3 && road_type[41] == 3 && road_type[42] == 3 && road_type[43] == 3 && road_type[44] == 3)
//             MotorPID.SpeedSet = s_max + 100; //������� δ���� �������ע��
//         else
//             MotorPID.SpeedSet = s_max;
//     }

//     else if (shortstraight_flag != 0)
//     {
//         road_type[0] = 2;
//         LedOff(1);
//         LedOff(3);
//         LedOff(2);
//         LedOn(1); //��ɫ

//         if (road_type[6] == 1 && road_type[7] == 1 && road_type[8] == 1 && road_type[9] == 1 && road_type[10] == 1)
//         { //ǰ8֡�ǳ���ֱ��,�ȼ���һ��
//             MotorPID.SpeedSet = s_zhi - 70;
//         }
//         else if (road_type[3] == 3 && road_type[4] == 3 && road_type[5] == 3)
//         { //ǰ3֡����� ����ŷķ���ڼ���
//             MotorPID.SpeedSet = s_wan1;
//         }
//         else
//         {
//             MotorPID.SpeedSet = (int16)(s_zhi - (s_zhi - s_wan1) * (abs(error_ave) / 35)); //ƫ�����35������ٶ�
//             MotorPID.SpeedSet = MotorPID.SpeedSet < s_wan1 ? s_wan1 : MotorPID.SpeedSet;
//         }
//     }

//     else if (f2.huanleftflag != 0 || f2.huanrightflag != 0 || f2.huan == 1)
//     {
//         road_type[0] = 5;
//         MotorPID.SpeedSet = s_huan;
//     }
//     else
//     {
//         road_type[0] = 3;
//         LedOff(1);
//         LedOff(3);
//         LedOff(2);
//         LedOn(1);
//         LedOn(3); //����ɫ

//         if (road_type[9] == 1 && road_type[10] == 1 && road_type[11] == 1 && road_type[12] == 1 && road_type[13] == 1)
//         { //ǰ6֡�ǳ���ֱ��,�ȼ���һ��
//             MotorPID.SpeedSet = s_wan1 - 30;
//         }
//         else
//         {
//             g_HighestSpeed = s_wan1;
//             g_LowestSpeed = s_wan2;
//             //��̬��������ٶȺ�����ٶȣ���������ֱ����Ǳ��
//             MotorPID.SpeedSet = g_HighestSpeed - (g_HighestSpeed - g_LowestSpeed) * (error_ave * error_ave) / (40 * 40); //ƫ�����40������ٶ�
//             MotorPID.SpeedSet = MotorPID.SpeedSet < g_LowestSpeed ? g_LowestSpeed : MotorPID.SpeedSet;
//             MotorPID.SpeedSet = MotorPID.SpeedSet > g_HighestSpeed ? g_HighestSpeed : MotorPID.SpeedSet; //��Сs_wan2���s_wan1
//         }
//     }

//     for (i = 200; i >= 1; i--)
//     {
//         road_type[i] = road_type[i - 1];
//     }
// }