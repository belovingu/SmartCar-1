#include "findLines.h"
#include "distinguish.h"

/*******十字标志，定义******/
int16 L = 0;         //左弯时补线后中线消失的行数
int16 R = 0;         //右弯时补线后中线消失的行数
int16 both_line = 0; //十字拐点消失时找到前面不丢线的行
int16 xie_flag = 0;
float k_left[3] = {0, 0, 0}; //十字补线斜率记录
float k_right[3] = {0, 0, 0};
float shizi_distance = 0;
int16 HL = 0, HR = 0;
/*******其余标志，定义******/
int16 n = 0;
int16 js1 = 0;
int16 js2 = 0;
int16 js3 = 0;
int16 js4 = 0;
int16 js5 = 0;
int16 js6 = 0;
int16 js7 = 0;
int16 js8 = 0;
int16 js9 = 0;
int16 js10 = 0;
int16 js11 = 0;
int16 js12 = 0;

Findline1_TypeDef f1;
Findline2_TypeDef f2;
int16 column_start = 80;
int16 half_width[120] =
    {4, 4, 4, 5, 5, 5, 6, 6, 7, 7,
     8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
     17, 18, 18, 19, 20, 20, 21, 22, 22, 23,
     23, 24, 24, 25, 25, 26, 26, 27, 28, 29,
     30, 30, 30, 31, 31, 32, 32, 33, 34, 34,
     34, 35, 36, 36, 37, 37, 38, 38, 39, 39,
     40, 40, 41, 41, 42, 43, 43, 44, 44, 45,
     46, 46, 47, 47, 48, 48, 49, 49, 50, 50,
     51, 51, 52, 52, 53, 53, 54, 55, 55, 56,
     56, 57, 57, 58, 58, 59, 60, 60, 61, 61,
     62, 62, 63, 63, 64, 65, 65, 66, 67, 67,
     68, 68, 69, 69, 70, 70, 71, 71, 72, 72};
// {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
//  0, 1, 1, 1, 1, 1, 2, 2, 2, 2,
//  3, 3, 3, 3, 4, 4, 4, 4, 5, 5,
//  5, 6, 6, 7, 7, 8, 9, 10, 11, 12,
//  13, 14, 15, 16, 18, 19, 19, 21, 22, 23,
//  24, 25, 27, 27, 29, 30, 31, 32, 32, 34,
//  35, 36, 37, 38, 39, 40, 41, 42, 43, 45,
//  46, 47, 48, 48, 50, 51, 52, 53, 54, 55,
//  56, 57, 58, 59, 60, 61, 62, 63, 65, 65,
//  67, 68, 69, 70, 71, 72, 73, 73, 75, 76,
//  77, 78, 78, 79, 79, 79, 79, 79, 79, 79,
//  79, 79, 79, 79, 79, 79, 79, 79, 79, 79};

// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 83, 84, 81, 77, 68, 50,
// 34, 34, 34, 36, 36, 38, 40, 40, 42, 44, 44, 46, 46, 48, 49,
// 51, 51, 53, 53, 55, 56, 58, 60, 61, 61, 63, 63, 65, 65, 67,
// 68, 69, 69, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81, 81, 83,
// 83, 85, 86, 87, 88, 89, 90, 92, 93, 94, 94, 96, 97, 98, 99,
// 100, 101, 102, 103, 105, 105, 106, 107, 108, 110, 110, 112,
// 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 125, 125,
// 127, 127, 128, 130, 131, 132, 134, 134, 136, 136, 138, 138,
// 140, 141, 142, 143, 144, 144

void findline1(void)
{
  int16 i = 0, j = 0, k = 0;
  f2.toppoint = 0;
  f2.toppoint_flag = 0;

  for (i = 118; i > 10; i--) //从118行一直找到10行
  {
    k = column_start;
    f1.leftline[i] = 0;
    f1.leftlineflag[i] = 0;
    f1.midline[i] = 80;
    f1.rightline[i] = 159;
    f1.rightlineflag[i] = 0;

    for (j = k; j > 2; j--) //Image[i][j]==0为黑，Image[i][j]!=0为白
    {
      if (Image[i][j] != 0 && Image[i][j - 1] != 0 && Image[i][j - 2] == 0)
      { //寻找左跳变点
        f1.leftline[i] = j - 1;
        f1.leftlineflag[i] = 1;
        break;
      }
    }
    for (j = k; j < 157; j++)
    {
      if (Image[i][j] != 0 && Image[i][j + 1] != 0 && Image[i][j + 2] == 0)
      { //寻找右跳变点
        f1.rightline[i] = j + 1;
        f1.rightlineflag[i] = 1;
        break;
      }
    }

    if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
    { //两边都找到跳变点
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1)
    { //左边无跳变点，右边有跳变点
      f1.leftline[i] = 0;
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      if (i < 99 && f1.rightline[i] <= f1.rightline[i + 1] && f1.leftlineflag[i + 15] == 0 && f1.rightlineflag[i + 15] == 0) //直线上不可能出现该情况，仅存在于转弯处
      {
        f1.midline[i] = f1.midline[i + 1];
      }
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    else if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 0)
    { //左边有跳变点，右边无跳变点
      f1.rightline[i] = 159;
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      if (i < 99 && f1.leftline[i] >= f1.leftline[i + 1] && f1.leftlineflag[i + 15] == 0 && f1.rightlineflag[i + 15] == 0)
      {
        f1.midline[i] = f1.midline[i + 1];
      }
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0) //&&f2.shiziflag[1]==0)
    {                                                             //左右无跳变点
      f1.leftline[i] = 0;
      f1.rightline[i] = 159;
      if (i == 118)
      {
        f1.midline[i] = 80;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1]; //用上一场的
      }
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    column_start = f1.midline[i];
  }

  column_start = f1.midline[118]; //每场之间的起始点设定

  if (f2.toppoint_flag == 0)
  {
    find_toppoint(); //寻找一场图像的赛道顶点
  }
}

// 弯道判断
// 十字判断
// 顶点判断
// 拐点判断
void findline2(void)
{
  int16 i = 0, m = 0;
  f2.leftturnflag = 0;
  f2.leftturn_row = 0;
  f2.rightturnflag = 0;
  f2.rightturn_row = 0;
  f2.leftguaiflag = 0;
  f2.leftguai_column = 0;
  f2.leftguai_row = 0;
  f2.rightguaiflag = 0;
  f2.rightguai_column = 0;
  f2.rightguai_row = 0;
  f2.leftguaiflag_up = 0;
  f2.leftguai_column_up = 0;
  f2.leftguai_row_up = 0;
  f2.rightguaiflag_up = 0;
  f2.rightguai_column_up = 0;
  f2.rightguai_row_up = 0;
  L = 0;
  R = 0;
  both_line = 0;
  f2.lefthuanguaiflag = 0;
  f2.lefthuanguai_column = 0;
  f2.lefthuanguai_row = 0;
  f2.righthuanguaiflag = 0;
  f2.righthuanguai_column = 0;
  f2.righthuanguai_row = 0;
  // f2.huanleftflag;f2.huanrightflag;
  //f2.huanleft_row=0;f2.huanright_row=0;
  // f2.huanleft_column=0;f2.huanright_column=0;

  if (f2.leftguaiflag == 0 && f2.rightguaiflag == 0)
  { //寻找赛道上是否存在拐点
    find_guai();
  }

  if (f2.toppoint != 0)
  { //有顶点时找小弯
    find_curve1();
  }

  if (f2.toppoint != 0 && f2.toppoint < 95 && f2.leftturnflag == 0 && f2.rightturnflag == 0)
  { //判断是否深弯
    find_curve3();
  }

  /********************************************十字判断*******************************************************************/
  if (f2.leftguaiflag == 1 || f2.rightguaiflag == 1)
  { //拐点存在
    find_huan1();
    shizi_judge();

    if (f2.leftguaiflag == 1 && f2.rightguaiflag == 1)
    {

      if (f2.leftguai_row > f2.rightguai_row && f2.leftguai_column > f2.rightguai_column)
      { //左拐点在右拐点右边时 保留近处拐点
        f2.rightguaiflag = 0;
      }
      if (f2.rightguai_row > f2.leftguai_row && f2.leftguai_column > f2.rightguai_column)
      {
        f2.leftguaiflag = 0;
      }
    }

    both_row_lose_judge(); //检查左右拐点的两边是否存在丢线
    if (f2.leftguaiflag == 1 || f2.rightguaiflag == 1)
    {

      if (f2.shiziflag[1] == 0)
      {
        shizi_distance = 0;
        f2.shiziflag[0] = f2.shiziflag[0] + 1;
      }
      if (f2.shiziflag[0] == 4) //满足4帧图像，确认为十字
      {
        f2.shiziflag[1] = 1;
        f2.shiziflag[0] = 0;
      }

      if (f2.leftturnflag != 0 || f2.rightturnflag != 0) //拐点存在，销去转弯标志位
      {
        f2.leftturnflag = 0;
        f2.rightturnflag = 0;
      }
    }

    if (f2.leftguaiflag == 0)
    {
      f2.leftguaiflag = 0;
      f2.leftguai_row = 0;
      f2.leftguai_column = 0;
    }
    if (f2.rightguaiflag == 0)
    {
      f2.rightguaiflag = 0;
      f2.rightguai_row = 0;
      f2.rightguai_column = 0;
    }
  }

  if (js1 == 1 || js2 == 1)
  {
    find_huan2();
  }

  if (js1 == 2 || js2 == 2)
  {
    find_huan3();
  }

  if (js1 == 3 || js2 == 3)
  {
    //BUZZER_ON;
    if (js2 == 3)
    {
      f2.huanrightflag = 1;
      js2 = 0;
    }
    if (js1 == 3)
    {
      f2.huanleftflag = 1;
      js1 = 0;
    }
  }

  /*************环补线***********/

  if (f2.huanleftflag != 0 || f2.huanrightflag != 0)
  {
    // HL = 0;
    // HR = 0;
    if (f2.huanleftflag != 0)
    {
      for (i = 100; i > f2.toppoint; i--)
      {
        if (((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 3] - f1.leftline[i + 3])) &&
            ((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i - 3] - f1.leftline[i - 3])) &&
            f1.leftlineflag[i - 3] == 1 && f1.leftlineflag[i + 3] == 1)
        {
          if (i >= 53 && f1.rightlineflag[i] != 0 &&
              f1.rightlineflag[i + 4] != 0 &&
              f1.rightlineflag[i - 4] != 0 &&
              f1.rightlineflag[i + 8] != 0 &&
              f1.rightlineflag[i - 8] != 0)
          {
            HL = 1;
            break;
          }
        }
      }
      if (HL == 0)
      {
        for (i = 117; i > f2.toppoint; i--)
        {
          if (f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1]; //环线直线部分中线逐渐往左偏
          }
          else
          {
            f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
          }
        }
      }
      if (HL == 1)
      {
        for (i = 115; i > 35; i--)
        {
          if (i < 85 && f1.leftlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 2] - 3; //丢线部分中线逐渐往左偏
          }
          else if (i >= 85 && f1.leftlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1];
          }
          else
          {
            f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]); //丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
          }
          if (f1.midline[i] <= 0)
          {
            f1.midline[i] = 0;
            L = i; //左弯时补线后中线消失的行数
            break;
          }
        }
        L = L == 0 ? f2.toppoint : L;
        for (i = L; i > 0; i--)
        {
          f1.leftline[i] = 0;
          f1.rightline[i] = 0;
          f1.midline[i] = 0;
        }
      }
      find_huanguai();

      //    if(f2.huanleft_column<40)
      //     {
      //       HL=1;
      //     }
      if (f2.righthuanguaiflag == 1)
      {
        if (f2.righthuanguai_column >= 125)
        {
          n = 0;
          f2.huanleftflag = 0;
          f2.lefthuanguaiflag = 0;
          //         f2.righthuanguaiflag=0;
          f2.huan = 1;
          js3 = 1;
        }
      }
      f2.leftturnflag = 0;
    }
    if (f2.huanrightflag != 0)
    {

      for (i = 100; i > f2.toppoint; i--)
      {
        if (((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 3] - f1.leftline[i + 3])) &&
            ((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i - 3] - f1.leftline[i - 3])) &&
            f1.rightlineflag[i - 3] == 1 && f1.rightlineflag[i + 3] == 1)
        {
          if (i >= 53 && f1.leftlineflag[i] != 0 &&
              f1.leftlineflag[i + 4] != 0 &&
              f1.rightlineflag[i - 4] != 0)
          {
            HR = 1;
            break;
          }
        }
      }
      if (HR == 0)
      {
        for (i = 117; i > f2.toppoint; i--)
        {
          if (f1.leftline[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1]; //环线直线部分中线逐渐往左偏
          }
          else
          {
            f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);
          }
        }
      }
      else if (HR == 1)
      {
        for (i = 115; i > 35; i--)
        {
          if (i < 85 && f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 2] + 3; //丢线部分中线逐渐往左偏
          }
          else if (i >= 85 && f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1];
          }
          else
          {
            f1.midline[i] = f1.rightline[i + 1] - (int16)(half_width[i]); //丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
          }
          if (f1.midline[i] >= 159)
          {
            f1.midline[i] = 159;
            R = i; //左弯时补线后中线消失的行数
            break;
          }
        }
        R = R == 0 ? f2.toppoint : R;
        for (i = R; i > 0; i--)
        {
          f1.leftline[i] = 159;
          f1.rightline[i] = 159;
          f1.midline[i] = 159;
        }
      }
      find_huanguai();
      //    if(f2.huanright_column>=120)
      //    {
      //      HR=1;
      //    }
      if (f2.lefthuanguaiflag == 1)
      {
        if (f2.lefthuanguai_column <= 35)
        {
          n = 0;
          f2.huanrightflag = 0;
          f2.righthuanguaiflag = 0;
          //         f2.lefthuanguaiflag=0;
          f2.huan = 1;
          js4 = 1;
        }
      }
      f2.rightturnflag = 0;
    }
    find_toppoint();
  }

  if (f2.huan == 1)
  {
    f2.shiziflag[1] = 0;
    f2.shiziflag[0] = 0;
    if (js3 == 1)
    {
      for (i = 95; i > f2.toppoint; i--)
      {
        if (f2.righthuanguaiflag == 0 && f1.rightline[i] > f1.rightline[i + 2] && f1.rightline[i] > f1.rightline[i + 3]) //&&f1.rightlineflag[i+5]==1(f1.rightline[i-3]-f1.leftline[i-3])>=150&&(f1.rightline[i-2]-f1.leftline[i-2])>=150
        {
          js7 = 1;
          js3 = 0;
          f2.huanleftflag = 0;
          break;
        }
      }
    }
    if (js4 == 1)
    {
      for (i = 95; i > f2.toppoint; i--)
      {
        if (f2.lefthuanguaiflag == 0 && f1.leftline[i] > f1.leftline[i + 2] && f1.leftline[i] > f1.leftline[i + 3]) //&&f1.leftlineflag[i+5]==1(f1.rightline[i-3]-f1.leftline[i-3])>=150&&(f1.rightline[i-2]-f1.leftline[i-2])>=150
        {
          js8 = 1;
          js4 = 0;
          f2.huanrightflag = 0;
          break;
        }
        //      }
      }
    }
  }

  if (js7 != 0 || js8 != 0)
  {
    f2.leftturnflag = 0;
    f2.rightturnflag = 0;
    m = 0;
    f2.shiziflag[1] = 0;
    f2.shiziflag[0] = 0;
    if (js7 == 1 && js8 == 0)
    {
      for (i = 110; i > f2.toppoint; i--)
      {
        if (f1.rightlineflag[i] == 0)
        {
          f1.midline[i] = f1.midline[i + 2] - 2;
        }
        else if (f1.rightline[i] > f1.rightline[i + 1] && f1.rightlineflag[i] != 0)
        {
          f1.midline[i] = f1.midline[i + 2] - 3;
        }
        else
        {
          f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]);
        }
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //左弯时补线后中线消失的行数
          break;
        }
      }
      L = L == 0 ? f2.toppoint : L;
      for (i = L; i > 0; i--)
      {
        f1.leftline[i] = 0;
        f1.rightline[i] = 0;
        f1.midline[i] = 0;
      }
    }
    else if (js8 == 1 && js7 == 0)
    {

      for (i = 110; i > 35; i--)
      {
        if (f1.leftlineflag[i] == 0)
        {
          f1.midline[i] = f1.midline[i + 1] + 3;
        }
        else if (f1.leftline[i] < f1.leftline[i + 1] && f1.leftlineflag[i] != 0)
        {
          f1.midline[i] = f1.midline[i + 2] + 3;
        }
        else
        {
          f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]);
        }
        if (f1.midline[i] >= 159)
        {
          f1.midline[i] = 159;
          R = i; //左弯时补线后中线消失的行数
          break;
        }
      }
      R = R == 0 ? f2.toppoint : L;
      for (i = R; i > 0; i--)
      {
        f1.leftline[i] = 159;
        f1.rightline[i] = 159;
        f1.midline[i] = 159;
      }
    }

    for (i = 95; i > 45; i--)
    {
      if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
      {
        m = m + 1;
      }
    }
    if (m > 40 && shortstraight_flag != 0)
    {
      js11 = 0;
      js12 = 0;
      f2.huan = 0;
      HL = 0;
      HR = 0;
      n = 0;
      m = 0;
      js7 = 0;
      js8 = 0;
    }
  }

  /**********************************十字补线***********************************************/
  // if (f2.shiziflag[1] == 1)
  // {
  //   if (f2.leftguaiflag == 1 && f2.rightguaiflag == 1)
  //   {
  //     if (f2.leftguai_row + 10 < 118)
  //     {
  //       k_left[0] = ((float)(f2.leftguai_column - f1.leftline[f2.leftguai_row + 10]) / 10.0);
  //       f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]); //加权左线斜率
  //     }
  //     if (f2.leftguai_row + 10 >= 118)
  //     {
  //       k_left[0] = ((float)(f2.leftguai_column - f1.leftline[118]) / (float)(118 - f2.leftguai_row));

  //       f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]); //加权左线斜率
  //     }

  //     if (f2.rightguai_row + 8 < 118)
  //     {
  //       k_right[0] = ((float)(f2.rightguai_column - f1.rightline[f2.rightguai_row + 8]) / 8.0);
  //       f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]); //加权右线斜率
  //     }
  //     if (f2.rightguai_row + 8 >= 118)
  //     {
  //       k_right[0] = ((float)(f2.rightguai_column - f1.rightline[118]) / (float)(118 - f2.rightguai_row));

  //       f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]); //加权右线斜率
  //     }

  //     for (i = f2.rightguai_row; i >= 10; i--)
  //     { //计算右边补线点
  //       f1.rightline[i] = (int16)(f2.rightguai_column + f2.zhizi_k_R * (f2.rightguai_row - i));
  //       if (f1.rightline[i] > 159)
  //       {
  //         f1.rightline[i] = 159;
  //       }
  //       if (f1.rightline[i] < 0)
  //       {
  //         f1.rightline[i] = 0;
  //       }
  //     }

  //     for (i = f2.leftguai_row; i >= 10; i--)
  //     { //计算左边补线点
  //       f1.leftline[i] = (int16)(f2.leftguai_column + f2.zhizi_k_L * (f2.leftguai_row - i));
  //       if (f1.leftline[i] > 159)
  //       {
  //         f1.leftline[i] = 159;
  //       }
  //       if (f1.leftline[i] < 0)
  //       {
  //         f1.leftline[i] = 0;
  //       }
  //     }
  //     for (i = MAX(f2.leftguai_row, f2.rightguai_row); i > 10; i--)
  //     {
  //       f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;

  //       if (f1.midline[i] > 159)
  //       {
  //         f1.midline[i] = 159;
  //       }
  //       if (f1.midline[i] < 0)
  //       {
  //         f1.midline[i] = 0;
  //       }
  //     }

  //     k_left[2] = k_left[1];
  //     k_left[1] = k_left[0];

  //     k_right[2] = k_right[1];
  //     k_right[1] = k_right[0];
  //   }

  //   if (f2.leftguaiflag == 0 && f2.rightguaiflag == 1)
  //   {
  //     if (f2.rightguai_row + 8 < 118)
  //     {
  //       k_right[0] = ((float)(f2.rightguai_column - f1.rightline[f2.rightguai_row + 8]) / 8.0);
  //       f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]);
  //     }
  //     if (f2.rightguai_row + 8 >= 118)
  //     {
  //       k_right[0] = ((float)(f2.rightguai_column - f1.rightline[118]) / (float)(118 - f2.rightguai_row));
  //       f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]);
  //     }
  //     for (i = f2.rightguai_row; i >= 10; i--)
  //     {
  //       f1.rightline[i] = (int16)(f2.rightguai_column + f2.zhizi_k_R * (f2.rightguai_row - i));
  //       if (f1.rightline[i] > 159)
  //       {
  //         f1.rightline[i] = 159;
  //       }
  //       if (f1.rightline[i] < 0)
  //       {
  //         f1.rightline[i] = 0;
  //       }
  //     }

  //     for (i = 118; i > 10; i--)
  //     {
  //       f1.midline[i] = f1.rightline[i] - (int16)(half_width[i]) - 10; //右边线补半宽

  //       if (f1.midline[i] > 159)
  //       {
  //         f1.midline[i] = 159;
  //       }
  //       if (f1.midline[i] < 0)
  //       {
  //         f1.midline[i] = 0;
  //       }
  //     }

  //     k_right[2] = k_right[1];
  //     k_right[1] = k_right[0];
  //   }

  //   if (f2.leftguaiflag == 1 && f2.rightguaiflag == 0)
  //   {
  //     if (f2.leftguai_row + 8 < 118)
  //     {
  //       k_left[0] = ((float)(f2.leftguai_column - f1.leftline[f2.leftguai_row + 8]) / 8.0);
  //       f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]);
  //     }
  //     if (f2.leftguai_row + 8 >= 118)
  //     {
  //       k_left[0] = ((float)(f2.leftguai_column - f1.leftline[118]) / (float)(118 - f2.leftguai_row));
  //       f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]);
  //     }
  //     for (i = f2.leftguai_row; i >= 10; i--)
  //     {
  //       f1.leftline[i] = (int16)(f2.leftguai_column + f2.zhizi_k_L * (f2.leftguai_row - i));
  //       if (f1.leftline[i] > 159)
  //       {
  //         f1.leftline[i] = 159;
  //       }
  //       if (f1.leftline[i] < 0)
  //       {
  //         f1.leftline[i] = 0;
  //       }
  //     }

  //     for (i = 118; i > 10; i--)
  //     {
  //       f1.midline[i] = f1.leftline[i] + (int16)(half_width[i]) + 10; //左边线补半宽

  //       if (f1.midline[i] > 159)
  //       {
  //         f1.midline[i] = 159;
  //       }
  //       if (f1.midline[i] < 0)
  //       {
  //         f1.midline[i] = 0;
  //       }
  //     }

  //     k_left[2] = k_left[1];
  //     k_left[1] = k_left[0];
  //   }
  //   if (f2.leftguaiflag == 0 && f2.rightguaiflag == 0)
  //   {
  //     for (i = 85; i > 55; i--)
  //     {
  //       if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
  //       {
  //         m = m + 1;
  //       }
  //     }
  //     if (m >= 25)
  //     {
  //       f2.shiziflag[1] = 0;
  //     }
  //   }
  //   find_toppoint(); //补完十字后重新定义一个新的顶点
  // }

  /***********************************/
  //  弯道补线
  /************************************/

  if (f2.leftturnflag != 0 || f2.rightturnflag != 0)
  {
    if (f2.leftturnflag == 1)
    {
      for (i = f2.leftturn_row; i > f2.toppoint; i--)
      {
        if (f1.rightline[i] == 159)
        {
          f1.midline[i] = f1.midline[i + 1] - 1; //丢线部分中线逐渐往左偏
        }
        else
        {
          f1.midline[i] = f1.midline[i + 1] - (f1.rightline[i + 1] - f1.rightline[i]); //丢线部分中线逐渐往左偏，且偏离的弧度跟右边界向左偏离程度相同
        }
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //左弯时补线后中线消失的行数
          break;
        }
      }
      L = L == 0 ? f2.toppoint : L;
      for (i = L; i > 0; i--)
      {
        f1.leftline[i] = 0;
        f1.rightline[i] = 0;
        f1.midline[i] = 0;
      }
    }

    if (f2.leftturnflag == 3)
    {
      for (i = 118; i > 0; i--)
      {
        f1.midline[i] = f1.rightline[i] - 80; //直接以右边界减去80，将中线轨迹向左偏离幅度更大
        f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
        f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i]; //限幅
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //左弯时补线后中线消失的行数
          break;
        }
      }
      L = L == 0 ? f2.toppoint : L;
      for (i = L; i > 0; i--)
      {
        f1.leftline[i] = 0;
        f1.rightline[i] = 0;
        f1.midline[i] = 0;
      }
    }

    if (f2.rightturnflag == 1)
    {
      for (i = f2.rightturn_row; i > 0; i--)
      {
        if (f1.leftline[i] == 0)
        {
          f1.midline[i] = f1.midline[i] + 1; //丢线部分，中线往右边偏
        }
        else
        {
          f1.midline[i] = f1.midline[i + 1] + (f1.leftline[i] - f1.leftline[i + 1]); //丢线部分中线逐渐往右偏，且偏离的弧度跟左边界向右偏离程度相同
        }

        if (f1.midline[i] >= 159)
        {
          f1.midline[i] = 159;
          R = i;
          break;
        }
      }
      R = R == 0 ? f2.toppoint : R;
      for (i = R; i > 0; i--)
      {
        f1.leftline[i] = 159;
        f1.rightline[i] = 159;
        f1.midline[i] = 159;
      }
    }

    if (f2.rightturnflag == 3)
    {
      for (i = 118; i > 0; i--)
      {
        f1.midline[i] = f1.leftline[i] + 80; //直接以左边界加80，将中线轨迹向右偏离幅度更大
        if (f1.midline[i] >= 159)
        {
          f1.midline[i] = 159;
          R = i;
          break;
        }
      }
      R = R == 0 ? f2.toppoint : R;
      for (i = R; i > 0; i--)
      {
        f1.leftline[i] = 159;
        f1.rightline[i] = 159;
        f1.midline[i] = 159;
      }
    }
  }

  find_toppoint();
  column_start = f1.midline[118]; //每场之间的起始点设定
  column_start = column_start > 150 ? 150 : column_start;
  column_start = column_start < 10 ? 10 : column_start;
} //f2.line的终点

void find_toppoint(void)
{
  int16 i = 0;
  for (i = 100; i > 5; i--)
  {
    if ((Image[i][f1.midline[i]] != 0) && (Image[i - 1][f1.midline[i]] == 0 || Image[i - 3][f1.midline[i]] == 0 || Image[i - 5][f1.midline[i]] == 0))
    {
      f2.toppoint = i;
      f2.toppoint_flag = 1;
      break;
    }
  }
  if (f2.toppoint_flag == 0)
  {
    for (i = 118; i > 2; i--)
    {
      if (f1.midline[i] > 150 || f1.midline[i] < 10)
      {
        f2.toppoint = i;
        f2.toppoint_flag = 1;
        break;
      }
    }
  }
}

void find_huan1(void)
{
  int i = 0;
  if (f2.leftguaiflag == 1 && f2.rightguaiflag == 0)
  {
    js1 = 1;
    js5 = f2.leftguai_row;
    for (i = 95; i > f2.toppoint; i--)
    {
      if (f1.rightlineflag[i] == 0)
      {
        js1 = 0;
      }
    }
  }
  else if (f2.leftguaiflag == 0 && f2.rightguaiflag == 1)
  {
    js2 = 1;
    js6 = f2.rightguai_row;
    for (i = 95; i > f2.toppoint; i--)
    {
      if (f1.leftlineflag[i] == 0)
      {
        js2 = 0;
      }
    }
  }
}

void find_huan2(void)
{
  int16 i = 0, cout = 0;
  for (i = 95; i > f2.toppoint; i--)
  {
    if (js1 == 1 && js2 == 0)
    {
      if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] != 0 && f1.rightlineflag[i + 8] != 0)
      {
        if (f1.leftlineflag[i + 4] == 1)
        {
          cout = cout + 1;
        }
      }
      if (cout >= 3)
      {
        js1 = 2;
        break;
      }
    }
    else if (js2 == 1 && js1 == 0)
    {
      if (f1.rightlineflag[i] == 0 && f1.leftlineflag[i] != 0 && f1.leftlineflag[i + 8] != 0)
      {
        if (f1.rightlineflag[i + 4] == 1)
        {
          cout = cout + 1;
        }
      }
      if (cout >= 3)
      {
        js2 = 2;
        break;
      }
    }
  }
  if (js1 == 1)
  {
    js1 = 0;
  }
  if (js2 == 1)
  {
    js2 = 0;
  }
}

void find_huan3(void)
{
  int16 i = 0, j = 0;
  for (i = 100; i > 35; i--)
  {
    if (js1 == 2 && js2 == 0)
    {
      if (f1.leftline[i] > f1.leftline[i + 3] &&
          f1.leftline[i] > f1.leftline[i - 3] &&
          f1.leftline[i] > f1.leftline[i + 5] &&
          f1.leftline[i] > f1.leftline[i - 5])
      {
        j = f1.midline[i + 5];
        if (Image[i][j] != 0 &&
            Image[i][j - 1] != 0 &&
            Image[i][j - 10] != 0 &&
            Image[i - 5][j] != 0 &&
            Image[i - 10][j] != 0 &&
            Image[i - 15][j] != 0 &&
            Image[i - 20][j] != 0 &&
            Image[i - 25][j] != 0 &&
            Image[i][j + 5] != 0)
        {
          if (f1.rightlineflag[i] == 1 && f1.rightlineflag[i - 3] == 1 && f1.rightlineflag[i + 3] == 1 && abs(f1.rightline[i] - f1.rightline[i - 3]) <= 6)
          {
            js1 = 3;
            f2.shiziflag[1] = 0;
            f2.shiziflag[0] = 0;
            break;
          }
        }
      }
    }
    else if (js1 == 0 && js2 == 2)
    {
      if (f1.rightline[i] < f1.rightline[i + 3] && f1.rightline[i] < f1.rightline[i - 3] && f1.rightline[i] < f1.rightline[i + 5] && f1.rightline[i] < f1.rightline[i - 5])
      {
        j = f1.midline[i + 5];
        if (Image[i][j] != 0 && Image[i][j - 1] != 0 && Image[i][j - 10] != 0 && Image[i - 5][j] != 0 && Image[i - 10][j] != 0 && Image[i - 15][j] != 0 && Image[i - 20][j] != 0 && Image[i - 25][j] != 0 && Image[i][j + 5] != 0)
        {
          if (f1.leftlineflag[i] == 1 && f1.leftlineflag[i - 3] == 1 && f1.leftlineflag[i + 3] == 1 && abs(f1.leftline[i] - f1.leftline[i - 3]) <= 6)
          {
            js2 = 3;
            f2.shiziflag[1] = 0;
            f2.shiziflag[0] = 0;
            break;
          }
        }
      }
    }
  }
  if (js1 == 2 || js2 == 2)
  {
    js1 = 0;
    js2 = 0;
  }
}

void find_huanguai(void)
{
  int16 i = 0;
  for (i = 100; i > 35; i--) //拐点要在顶点之前
  {
    if (f2.lefthuanguaiflag == 0)
    {
      if (f1.leftline[i] < 80 &&
          f1.leftline[i] > f1.leftline[i + 3] &&
          f1.leftline[i] < f1.leftline[i - 3] &&
          f1.leftline[i] > f1.leftline[i + 5] &&
          f1.leftlineflag[i] == 1 &&
          f1.leftlineflag[i + 2] == 0)
      { //i>45是为了控制拐点在40行到115行内
        if (f1.leftline[i - 3] - f1.leftline[i + 3] > 12 &&
            f1.leftlineflag[i - 2] == 1 && f1.leftlineflag[i] == 1 &&
            f1.leftlineflag[i + 3] == 0)
        {
          if (2 * f1.leftline[i] - f1.leftline[i - 2] - f1.leftline[i + 2] > 8)
          { //寻找左拐点
            f2.lefthuanguaiflag = 1;
            f2.lefthuanguai_row = i;
            f2.lefthuanguai_column = f1.leftline[i];
            break;
          }
        }
      }
    }

    if (f2.righthuanguaiflag == 0)
    {
      if (f1.rightline[i] > 70 &&
          f1.rightline[i] < f1.rightline[i + 3] &&
          f1.rightline[i] > f1.rightline[i - 3] &&
          f1.rightline[i] < f1.rightline[i + 5] &&
          f1.rightlineflag[i] == 1 &&
          f1.rightlineflag[i + 2] == 0)
      { //i>45是为了控制拐点在40行到115行内
        if (f1.rightline[i - 3] - f1.rightline[i + 3] < -12 &&
            f1.rightlineflag[i - 2] == 1 &&
            f1.rightlineflag[i + 3] == 0 &&
            f1.rightlineflag[i] == 1)
        {
          if (f1.rightline[i + 2] + f1.rightline[i - 2] - 2 * f1.rightline[i] > 8)
          { //寻找右拐点
            f2.righthuanguaiflag = 1;
            f2.righthuanguai_row = i;
            f2.righthuanguai_column = f1.rightline[i];
            break;
          }
        }
      }
    }
  }
}

//找左右小弯
void find_curve1(void)
{
  int i = 0, lose_cnt;
  for (i = 115; i > f2.toppoint + 3; i--)
  {
    if (f1.leftlineflag[i] == 0)
    {
      if (f2.leftturnflag == 0 && f1.leftlineflag[i + 1] == 1 && f1.leftlineflag[i + 2] == 1 && f1.rightlineflag[i] == 1)
      { //找到左边开始丢线的那一行
        if (f1.rightlineflag[i - 5] == 1 && i > 30)
        {
          f2.leftturn_row = i; //是个小弯
          f2.leftturnflag = 1;
          if (f2.leftturn_row < 40)
          { //行数很靠前时防止在十字后远方看过去的时候误判
            if (f1.rightline[f2.leftturn_row - 5] > 100 && f1.rightline[f2.leftturn_row - 5] > f1.rightline[f2.leftturn_row])
            {
              if (f1.rightline[f2.leftturn_row - 5] - f1.rightline[f2.leftturn_row] > 10)
              {
                f2.rightturn_row = 0;
                f2.rightturnflag = 0;
              }
            }
          }
          if (f2.shiziflag[1] == 1 && f2.leftturnflag == 1) //防止十字给小弯道判断带来的影响
          {
            for (i = f2.leftturn_row; i > f2.toppoint; i--)
            { //右边不能丢线太多，防止丢十字进去后拐点丢失的几帧误判
              if (f1.rightlineflag[i] == 0)
              {
                lose_cnt++;
              }
            }
            if (lose_cnt > 5)
            {
              f2.leftturn_row = 0; //是个小弯
              f2.leftturnflag = 0;
            }
          }
          break;
        }
      }
    }

    else if (f1.rightlineflag[i] == 0)
    {
      if (f2.rightturnflag == 0 && f1.rightlineflag[i + 1] == 1 && f1.rightlineflag[i + 2] == 1 && f1.leftlineflag[i] == 1)
      {
        if (f1.leftlineflag[i - 5] == 1 && i > 30)
        {
          f2.rightturn_row = i;
          f2.rightturnflag = 1;
          if (f2.rightturn_row <= 40)
          { //行数很靠前时防止在十字后远方看过去的时候误判
            if (f1.leftline[f2.rightturn_row - 5] < 60 && f1.leftline[f2.rightturn_row - 5] < f1.leftline[f2.rightturn_row])
            {
              if (f1.leftline[f2.rightturn_row] - f1.leftline[f2.rightturn_row - 5] > 10)
              {
                f2.leftturn_row = 0;
                f2.leftturnflag = 0;
              }
            }
          }
          if (f2.shiziflag[1] == 1)
          {
            for (i = f2.rightturn_row; i > f2.toppoint; i--)
            { //右边不能丢线太多，防止丢十字进去后拐点丢失的几帧误判
              if (f1.leftlineflag[i] == 0)
              {
                lose_cnt++;
              }
            }
            if (lose_cnt > 5)
            {
              f2.rightturn_row = 0; //是个小弯
              f2.rightturnflag = 0;
            }
          }
          break;
        }
      }
    }
  }
}

//左右深弯
void find_curve3(void)
{
  int i = 0, flag_L = 0, flag_R = 0;
  if (f2.leftturnflag == 0 && f2.rightturnflag == 0)
  {
    for (i = 118; i > 112; i--) //扩大搜索范围
    {
      if (f1.leftlineflag[i] == 0 && flag_L == 0) //右线118-112行有丢线
      {
        flag_L = 1;
      }
      if (f1.rightlineflag[i] == 0 && flag_R == 0) //左线118-112行有丢线
      {
        flag_R = 1;
      }
    }
    if (flag_L == 1)
    {
      for (i = 100; i > f2.toppoint + 10; i--) //100减到90，放松条件，提前转弯
      {
        if (f1.leftlineflag[i] == 1 || f1.rightlineflag[i] == 0)
        {
          flag_L = 0;
        }
      }
    }
    if (flag_R == 1)
    {
      for (i = 100; i > f2.toppoint + 10; i--) //100减到90，放松条件，提前转弯
      {
        if (f1.leftlineflag[i] == 0 || f1.rightlineflag[i] == 1)
        {
          flag_R = 0;
        }
      }
    }
    if (flag_L == 1 && flag_R == 0)
    {
      f2.leftturnflag = 3;
    }
    else if (flag_L == 0 && flag_R == 1)
    {
      f2.rightturnflag = 3;
    }
  }
}

//找拐点 ，拐点是十字的特征
void find_guai(void)
{
  int16 i = 0;
  if (f2.toppoint < 70 && f2.toppoint > 5)
  {
    for (i = 108; i > f2.toppoint + 4; i--) //拐点要在顶点之前
    {
      if (f2.leftguaiflag == 0)
      {
        if (i > 45 && f1.leftline[i] < 140 && f1.leftline[i] > f1.leftline[i + 3] && f1.leftline[i] > f1.leftline[i - 3] && f1.leftline[i] > f1.leftline[i + 5] && f1.leftline[i] > f1.leftline[i - 2] && f1.leftline[i] > f1.leftline[i - 4])
        { //i>45是为了控制拐点在40行到115行内
          if (f1.leftline[i - 3] - f1.leftline[i + 3] < 2 && f1.leftlineflag[i + 3] == 1 && f1.leftlineflag[i - 5] == 0 && f1.leftlineflag[i] == 1)
          {
            if (2 * f1.leftline[i] - f1.leftline[i - 3] - f1.leftline[i + 3] > 4)
            { //寻找左拐点
              f2.leftguaiflag = 1;
              f2.leftguai_row = i;
              f2.leftguai_column = f1.leftline[i];
            }
          }
        }
      }
    }

    for (i = 108; i > f2.toppoint + 4; i--) //拐点要在顶点之前
    {

      if (f2.rightguaiflag == 0)
      {
        if (i > 45 && f1.rightline[i] > 20 && f1.rightline[i] < f1.rightline[i + 3] && f1.rightline[i] < f1.rightline[i - 3] && f1.rightline[i] < f1.rightline[i + 5])
        { //i>45是为了控制拐点在40行到115行内
          if (f1.rightline[i - 3] - f1.rightline[i + 3] > 2 && f1.rightlineflag[i + 3] == 1 && f1.rightlineflag[i - 5] == 0)
          {
            if (f1.rightline[i + 3] + f1.rightline[i - 3] - 2 * f1.rightline[i] > 4)
            { //寻找右拐点
              f2.rightguaiflag = 1;
              f2.rightguai_row = i;
              f2.rightguai_column = f1.rightline[i];
            }
          }
        }
      }
    }
  }
}

//十字判断
void shizi_judge(void)
{
  if (f2.leftguaiflag == 1)
  {
    if (f2.leftguai_column > 140)
    //在左边拐弯的标志为1的前提下，左拐列数太大，不宜再进行左拐，应将左拐标志取消
    {
      f2.leftguaiflag = 0;
      f2.leftguai_row = 0;
      f2.leftguai_column = 0;
    }
    //或者在此时右边没有丢线的情况下，左拐标志应为零（十字时候，才存在既有左拐点，同时在左拐点对应的行数上面判断右边线丢线）
    else if (right_row_lose_judge(f2.leftguai_row))
    {
      f2.leftguaiflag = 0;
      f2.leftguai_row = 0;
      f2.leftguai_column = 0;
    }
  }

  if (f2.rightguaiflag == 1)
  {
    if (f2.rightguai_column < 20)
    {
      f2.rightguaiflag = 0;
      f2.rightguai_row = 0;
      f2.rightguai_column = 0;
    }
    else if (left_row_lose_judge(f2.rightguai_row))
    {
      f2.rightguaiflag = 0;
      f2.rightguai_row = 0;
      f2.rightguai_column = 0;
    }
  }
}

//分别判断是否丢线
int8 judge_leftlose(void)
{
  int16 i = 0, count = 0, flag = 0;
  for (i = 118; i > 40; i--)
  {
    if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1) //左边界存在，右边界不存在
    {
      count++; //计数加一
    }
  }
  if (count > 35)
  {
    flag = 1; //如果左边界丢线场数大于35行，左丢线标志置一
  }
  return flag;
}

int8 judge_rightlose(void)
{
  int16 i = 0, count = 0, flag = 0;
  for (i = 118; i > 40; i--)
  {
    if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 0)
    {
      count++;
    }
  }
  if (count > 35)
  {
    flag = 1;
  }
  return flag;
}

int8 judge_bothlose(void)
{
  int16 i = 0, count = 0, flag = 0;
  for (i = 118; i > 40; i--)
  {
    if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0) //如果左右边界均为0
    {
      count++;
    }
  }
  if (count > 10) //且丢线场数大于10，这认定此时已经丢线
  {
    flag = 1;
  }
  return flag;
}

//判断第row行右边线是否丢线
int8 right_row_lose_judge(int16 row)
{
  int16 i = 0, flag = 0;
  flag = 1;
  for (i = row; i > f2.toppoint; i--)
  {
    if (f1.rightlineflag[i] == 0)
    {
      flag = 0;
      break;
    }
  }
  return flag;
}

//判断第row行左边线上边是否丢线
int8 left_row_lose_judge(int16 row)
{
  int16 i = 0, flag = 0;
  flag = 1;
  for (i = row; i > f2.toppoint; i--)
  {
    if (f1.leftlineflag[i] == 0)
    {
      flag = 0;
      break;
    }
  }
  return flag;
}

//判断第row行左右边线上边是否丢线,检查左右拐点的两边是否存在丢线
void both_row_lose_judge(void)
{
  int16 i = 0, flag = 0;
  if (f2.leftguaiflag == 1)
  {
    flag = 1;
    for (i = f2.leftguai_row + 5; i > f2.leftguai_row - 20; i--)
    {
      if (f1.leftlineflag[i] == 0 || f1.rightlineflag[i] == 0)
      {
        flag = 0;
        break;
      }
    }
    if (flag == 1) //如果左或右拐点有丢线，原有拐点标志不变，如果没有丢线，原有拐点标志变为0
    {
      f2.leftguaiflag = 0;
    }
  }
  if (f2.rightguaiflag == 1)
  {
    flag = 1;
    for (i = f2.rightguai_row + 5; i > f2.rightguai_row - 20; i--)
    {
      if (f1.leftlineflag[i] == 0 || f1.rightlineflag[i] == 0)
      {
        flag = 0;
        break;
      }
    }
    if (flag == 1)
    {
      f2.rightguaiflag = 0;
    }
  }
}
