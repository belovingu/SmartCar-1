
#include "findLines.h"

/*******ʮ�ֱ�־������******/
int16 shizi_time = 0; //ʮ�ּ�ʱ��
int16 shizi;          //ʮ�ּ�����
int16 shizidistance;  //ʮ�־������
int16 L = 0;          //����ʱ���ߺ�������ʧ������
int16 R = 0;          //����ʱ���ߺ�������ʧ������
int16 both_line = 0;  //ʮ�ֹյ���ʧʱ�ҵ�ǰ�治���ߵ���
int16 xie_flag = 0;
float k_left[3] = {0, 0, 0}; //ʮ�ֲ���б�ʼ�¼
float k_right[3] = {0, 0, 0};
float shizi_distance = 0;
int16 start_cnt_distance = 0;
int16 shizi_cnt = 0;
int16 HL = 0, HR = 0;
/*******�����־������******/
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

int16 set_startline = 25;
Findline1_TypeDef f1;
Findline2_TypeDef f2;
int16 column_start = 80;
int16 text_width[120] = {0};
int16 width_real[120] =
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, //������� 1 2��Ϊ����ֵ ������Ҫ�����Լ��ĳ����в���
     0, 1, 1, 1, 1, 1, 2, 2, 2, 2,
     3, 3, 3, 3, 4, 4, 4, 4, 5, 5,
     5, 6, 6, 7, 7, 8, 9, 10, 11, 12,
     13, 14, 15, 16, 18, 19, 19, 21, 22, 23,
     24, 25, 27, 27, 29, 30, 31, 32, 32, 34,
     35, 36, 37, 38, 39, 40, 41, 42, 43, 45,
     46, 47, 48, 48, 50, 51, 52, 53, 54, 55,
     56, 57, 58, 59, 60, 61, 62, 63, 65, 65,
     67, 68, 69, 70, 71, 72, 73, 73, 75, 76,
     77, 78, 78, 79, 79, 79, 79, 79, 79, 79,
     79, 79, 79, 79, 79, 79, 79, 79, 79, 79};
int16 width[120] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, //����ȫ�� ������Ҫ�����Լ��ĳ����в���
     1, 1, 1, 2, 2, 3, 4, 4, 5, 7,
     7, 7, 7, 7, 8, 8, 8, 9, 10, 11,                    //28-18
     12, 13, 14, 14, 15, 17, 19, 21, 22, 26,            //38-28
     27, 29, 31, 33, 36, 38, 39, 42, 45, 47,            //48-38
     48, 51, 54, 55, 58, 60, 62, 64, 66, 68,            //58-48
     70, 72, 74, 77, 79, 81, 82, 85, 87, 90,            //68-58
     92, 94, 96, 97, 100, 102, 105, 107, 109, 111,      //78-68
     113, 114, 117, 119, 121, 123, 125, 127, 130, 131,  //88-78
     134, 136, 138, 140, 142, 144, 146, 148, 150, 152,  //98-88
     155, 156, 157, 159, 159, 159, 159, 159, 159, 159,  //108-98
     159, 159, 159, 159, 159, 159, 159, 159, 159, 159}; //118-108

void findline1(void)
{
  int16 i = 0, j = 0, k = 0;
  f2.toppoint = 0;
  f2.toppoint_flag = 0;

  for (i = 118; i > 10; i--) //��118��һֱ�ҵ�10��
  {
    k = column_start;
    f1.leftline[i] = 0;
    f1.leftlineflag[i] = 0;
    f1.midline[i] = 0;
    f1.rightline[i] = 0;
    f1.rightlineflag[i] = 0;

    for (j = k; j > 2; j--) //Image[i][j]==0Ϊ�ڣ�Image[i][j]!=0Ϊ��
    {
      if (Image[i][j] != 0 && Image[i][j - 1] != 0 && Image[i][j - 2] == 0)
      { //Ѱ���������
        f1.leftline[i] = j - 1;
        f1.leftlineflag[i] = 1;
        break;
      }
    }
    for (j = k; j < 157; j++)
    {
      if (Image[i][j] != 0 && Image[i][j + 1] != 0 && Image[i][j + 2] == 0)
      { //Ѱ���������
        f1.rightline[i] = j + 1;
        f1.rightlineflag[i] = 1;
        break;
      }
    }

    if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
    { //���߶��ҵ������
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    else if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1)
    { //���������㣬�ұ��������
      f1.leftline[i] = 0;
      f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;
      if (i < 99 && f1.rightline[i] <= f1.rightline[i + 1] && f1.leftlineflag[i + 15] == 0 && f1.rightlineflag[i + 15] == 0) //ֱ���ϲ����ܳ��ָ��������������ת�䴦
      {
        f1.midline[i] = f1.midline[i + 1];
      }
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    else if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 0)
    { //���������㣬�ұ��������
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
    {                                                             //�����������
      f1.leftline[i] = 0;
      f1.rightline[i] = 159;
      if (i == 118)
      {
        f1.midline[i] = 80;
      }
      else
      {
        f1.midline[i] = f1.midline[i + 1]; //����һ����
      }
      f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
      f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i];
    }
    column_start = f1.midline[i];
  }

  column_start = f1.midline[118]; //ÿ��֮�����ʼ���趨

  if (f2.toppoint_flag == 0)
  {
    find_toppoint(); //Ѱ��һ��ͼ�����������
  }
}

// ����ж�
// ʮ���ж�
// �����ж�
// �յ��ж�
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
  { //Ѱ���������Ƿ���ڹյ�
    find_guai();
  }

  if (f2.toppoint != 0)
  { //�ж���ʱ��С��
    find_curve1();
  }

  if (f2.toppoint != 0 && f2.toppoint < 95 && f2.leftturnflag == 0 && f2.rightturnflag == 0)
  { //�ж��Ƿ�����
    find_curve3();
  }

  /********************************************ʮ���ж�*******************************************************************/
  if (f2.leftguaiflag == 1 || f2.rightguaiflag == 1)
  { //�յ����
    find_huan1();
    shizi_judge();

    if (f2.leftguaiflag == 1 && f2.rightguaiflag == 1)
    {

      if (f2.leftguai_row > f2.rightguai_row && f2.leftguai_column > f2.rightguai_column)
      { //��յ����ҹյ��ұ�ʱ ���������յ�
        f2.rightguaiflag = 0;
      }
      if (f2.rightguai_row > f2.leftguai_row && f2.leftguai_column > f2.rightguai_column)
      {
        f2.leftguaiflag = 0;
      }
    }

    both_row_lose_judge(); //������ҹյ�������Ƿ���ڶ���
    if (f2.leftguaiflag == 1 || f2.rightguaiflag == 1)
    {

      if (f2.shiziflag[1] == 0)
      {
        shizi_distance = 0;
        f2.shiziflag[0] = f2.shiziflag[0] + 1;
      }
      if (f2.shiziflag[0] == 4) //����4֡ͼ��ȷ��Ϊʮ��
      {
        f2.shiziflag[1] = 1;
        f2.shiziflag[0] = 0;
      }

      if (f2.leftturnflag != 0 || f2.rightturnflag != 0) //�յ���ڣ���ȥת���־λ
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

  /*************������***********/

  if (f2.huanleftflag != 0 || f2.huanrightflag != 0)
  {
    if (f2.huanleftflag != 0)
    {
      for (i = 100; i > 35; i--)
      {
        if (((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 4] - f1.leftline[i + 4])) && ((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i - 4] - f1.leftline[i - 4])) && f1.leftlineflag[i - 4] == 1 && f1.leftlineflag[i + 4] == 1)
        {
          if (i >= 51 && f1.rightlineflag[i] != 0 && f1.rightlineflag[i + 4] != 0)
          {
            HL = 1;
            break;
          }
        }
        //       if(abs(f1.rightline[i]-f1.leftline[i]-width[i])<=10&&i>71&&abs(f1.rightline[i+5]-f1.leftline[i+5]-width[i+5])>=13)
        //       {
        //         n=1;
        //       }
      }
      if (HL == 0)
      {
        for (i = 117; i > f2.toppoint; i--)
        {
          if (f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1]; //����ֱ�߲�������������ƫ
          }
          else
          {
            f1.midline[i] = f1.rightline[i] - (int16)(width_real[i]) - 1;
          }
        }
      }
      if (HL == 1)
      {
        for (i = 115; i > 35; i--)
        {
          if (i <= 83 && f1.leftlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 2] - 5; //���߲�������������ƫ
          }
          else if (i > 83 && f1.leftlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1];
          }
          else
          {
            f1.midline[i] = f1.leftline[i] + (int16)(width_real[i]) - 1; //���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
          }
          if (f1.midline[i] <= 0)
          {
            f1.midline[i] = 0;
            L = i; //����ʱ���ߺ�������ʧ������
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
        if (f2.righthuanguai_column >= 120)
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
        if (((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i + 3] - f1.leftline[i + 3])) && ((f1.rightline[i] - f1.leftline[i]) < (f1.rightline[i - 3] - f1.leftline[i - 3])) && f1.rightlineflag[i - 3] == 1 && f1.rightlineflag[i + 3] == 1)
        {
          if (i >= 53 && f1.leftlineflag[i] != 0 && f1.leftlineflag[i + 4] != 0 && f1.rightlineflag[i - 4] != 0)
          {
            HR = 1;
            break;
          }
        }
        //          if(abs(f1.rightline[i]-f1.leftline[i]-width[i])<=10&&i>88)
        //          {
        //             n=1;
        //          }
      }
      if (HR == 0)
      {
        for (i = 117; i > f2.toppoint; i--)
        {
          if (f1.leftline[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1]; //����ֱ�߲�������������ƫ
          }
          else
          {
            f1.midline[i] = f1.leftline[i] + (int16)(width_real[i]);
          }
        }
      }
      else if (HR == 1)
      {
        for (i = 115; i > 35; i--)
        {
          if (i < 85 && f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 2] + 3; //���߲�������������ƫ
          }
          else if (i >= 85 && f1.rightlineflag[i] == 0)
          {
            f1.midline[i] = f1.midline[i + 1];
          }
          else
          {
            f1.midline[i] = f1.rightline[i + 1] - (int16)(width_real[i]) + 1; //���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
          }
          if (f1.midline[i] >= 159)
          {
            f1.midline[i] = 159;
            R = i; //����ʱ���ߺ�������ʧ������
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
          //        if(f1.rightline[i]-f1.rightline[i+4]>=14)
          //        {
          js7 = 1;
          js3 = 0;
          f2.huanleftflag = 0;
          break;
          //        }
        }
      }
    }
    if (js4 == 1)
    {
      for (i = 95; i > f2.toppoint; i--)
      {
        if (f2.lefthuanguaiflag == 0 && f1.leftline[i] > f1.leftline[i + 2] && f1.leftline[i] > f1.leftline[i + 3]) //&&f1.leftlineflag[i+5]==1(f1.rightline[i-3]-f1.leftline[i-3])>=150&&(f1.rightline[i-2]-f1.leftline[i-2])>=150
        {
          //         if((f1.leftline[i]-f1.leftline[i+4])<=-14)
          //         {
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
          f1.midline[i] = f1.midline[i + 2] - 1;
        }
        else if (f1.rightline[i] > f1.rightline[i + 1] && f1.rightlineflag[i] != 0)
        {
          f1.midline[i] = f1.midline[i + 2] - 3;
        }
        else
        {
          f1.midline[i] = f1.rightline[i] - (int16)(width_real[i]);
        }
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //����ʱ���ߺ�������ʧ������
          break;
        }
        //       if((f1.leftline[i]-f1.leftline[i+4])>15&&f1.leftlineflag[i]==1&&f1.leftlineflag[i+4]==0)
        //       {
        //         js11=1;
        //       }
        //       if(js11==1)
        //       {
        //         f1.midline[i]=f1.midline[i+1]-(f1.rightline[i+1]-f1.rightline[i]);
        //       }
        //       else
        //       {
        //          f1.midline[i]=f1.rightline[i]-(int16)(width_real[i])+1;
        //       }
        //       if(f1.midline[i]<=0)
        //         {
        //           f1.midline[i]=0;
        //         }
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

      //     for(i=110;i>35;i--)
      //     {
      //       if(f1.leftlineflag[i]==1&&f1.leftlineflag[i-2]==0&&f1.leftlineflag[i+2]==1)
      //       {
      //         js11=i;
      //         js12=((float)(f1.leftline[i-5]-f1.leftline[i]))/12.0;
      //         break;
      //       }

      //       if((f1.rightline[i]-f1.rightline[i+4])<-15&&f1.rightlineflag[i]==1&&f1.rightlineflag[i+4]==0)
      //       {
      //         js12=1;
      //       }
      //       if(js12==1)
      //       {
      //         f1.midline[i]=f1.midline[i+1]+(f1.leftline[i]-f1.leftline[i+1]);
      //       }
      //       else
      //       {
      //         f1.midline[i]=f1.leftline[i]+(int16)(width_real[i])-1;
      //       }
      //       if(f1.midline[i]>=159)
      //         {
      //           f1.midline[i]=159;
      //         }
      //    }
      for (i = 110; i > 35; i--)
      {
        if (f1.leftlineflag[i] == 0)
        {
          f1.midline[i] = f1.midline[i + 1] + 1;
        }
        else if (f1.leftline[i] < f1.leftline[i + 1] && f1.leftlineflag[i] != 0)
        {
          f1.midline[i] = f1.midline[i + 2] + 3;
        }
        else
        {
          f1.midline[i] = f1.leftline[i] + (int16)(width_real[i]) + 1;
        }
        if (f1.midline[i] >= 159)
        {
          f1.midline[i] = 159;
          R = i; //����ʱ���ߺ�������ʧ������
          break;
        }
        //       if(f1.leftlineflag[i]!=0&&i>js11)
        //       {
        //         f1.midline[i]=f1.leftline[i]+(int16)(width_real[i]);
        //       }
        //       if(f1.leftlineflag[i]!=0&&i<=js11)
        //       {
        //         f1.midline[i]=(int16)(f1.midline[i+1]+(i-js11)*js12)-1;
        //       }
        //       if(f1.midline[i]>=159)
        //       {
        //         f1.midline[i]=159;
        //       }
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
  //  for(i=100;i>50;i--)
  //  {
  //     if(f1.leftlineflag[i]==1&&f1.rightlineflag[i]==1)
  //     {
  //        m=m+1;
  //     }
  //     if(m>45)
  //     {
  //       js11=0;
  //       js12=0;
  //       f2.huan=0;
  //       HL=0;
  //       HR=0;
  //       n=0;
  //       js7=0;
  //       js8=0;
  //       break;
  //      }
  //   }
  /**********************************ʮ�ֲ���***********************************************/
  if (f2.shiziflag[1] == 1)
  {
    if (f2.leftguaiflag == 1 && f2.rightguaiflag == 1)
    {
      if (f2.leftguai_row + 10 < 118)
      {
        k_left[0] = ((float)(f2.leftguai_column - f1.leftline[f2.leftguai_row + 10]) / 10.0);
        f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]); //��Ȩ����б��
      }
      if (f2.leftguai_row + 10 >= 118)
      {
        k_left[0] = ((float)(f2.leftguai_column - f1.leftline[118]) / (float)(118 - f2.leftguai_row));

        f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]); //��Ȩ����б��
      }

      if (f2.rightguai_row + 8 < 118)
      {
        k_right[0] = ((float)(f2.rightguai_column - f1.rightline[f2.rightguai_row + 8]) / 8.0);
        f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]); //��Ȩ����б��
      }
      if (f2.rightguai_row + 8 >= 118)
      {
        k_right[0] = ((float)(f2.rightguai_column - f1.rightline[118]) / (float)(118 - f2.rightguai_row));

        f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]); //��Ȩ����б��
      }

      for (i = f2.rightguai_row; i >= 10; i--)
      { //�����ұ߲��ߵ�
        f1.rightline[i] = (int16)(f2.rightguai_column + f2.zhizi_k_R * (f2.rightguai_row - i));
        if (f1.rightline[i] > 159)
        {
          f1.rightline[i] = 159;
        }
        if (f1.rightline[i] < 0)
        {
          f1.rightline[i] = 0;
        }
      }

      for (i = f2.leftguai_row; i >= 10; i--)
      { //������߲��ߵ�
        f1.leftline[i] = (int16)(f2.leftguai_column + f2.zhizi_k_L * (f2.leftguai_row - i));
        if (f1.leftline[i] > 159)
        {
          f1.leftline[i] = 159;
        }
        if (f1.leftline[i] < 0)
        {
          f1.leftline[i] = 0;
        }
      }
      for (i = MAX(f2.leftguai_row, f2.rightguai_row); i > 10; i--)
      {
        f1.midline[i] = (f1.leftline[i] + f1.rightline[i]) / 2;

        if (f1.midline[i] > 159)
        {
          f1.midline[i] = 159;
        }
        if (f1.midline[i] < 0)
        {
          f1.midline[i] = 0;
        }
      }

      k_left[2] = k_left[1];
      k_left[1] = k_left[0];

      k_right[2] = k_right[1];
      k_right[1] = k_right[0];
    }

    if (f2.leftguaiflag == 0 && f2.rightguaiflag == 1)
    {
      if (f2.rightguai_row + 8 < 118)
      {
        k_right[0] = ((float)(f2.rightguai_column - f1.rightline[f2.rightguai_row + 8]) / 8.0);
        f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]);
      }
      if (f2.rightguai_row + 8 >= 118)
      {
        k_right[0] = ((float)(f2.rightguai_column - f1.rightline[118]) / (float)(118 - f2.rightguai_row));
        f2.zhizi_k_R = (float)(0.6 * k_right[0] + 0.2 * k_right[1] + 0.2 * k_right[2]);
      }
      for (i = f2.rightguai_row; i >= 10; i--)
      {
        f1.rightline[i] = (int16)(f2.rightguai_column + f2.zhizi_k_R * (f2.rightguai_row - i));
        if (f1.rightline[i] > 159)
        {
          f1.rightline[i] = 159;
        }
        if (f1.rightline[i] < 0)
        {
          f1.rightline[i] = 0;
        }
      }

      for (i = 118; i > 10; i--)
      {
        f1.midline[i] = f1.rightline[i] - (int16)(width_real[i]) - 10; //�ұ��߲����

        if (f1.midline[i] > 159)
        {
          f1.midline[i] = 159;
        }
        if (f1.midline[i] < 0)
        {
          f1.midline[i] = 0;
        }
      }

      k_right[2] = k_right[1];
      k_right[1] = k_right[0];
    }

    if (f2.leftguaiflag == 1 && f2.rightguaiflag == 0)
    {
      if (f2.leftguai_row + 8 < 118)
      {
        k_left[0] = ((float)(f2.leftguai_column - f1.leftline[f2.leftguai_row + 8]) / 8.0);
        f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]);
      }
      if (f2.leftguai_row + 8 >= 118)
      {
        k_left[0] = ((float)(f2.leftguai_column - f1.leftline[118]) / (float)(118 - f2.leftguai_row));
        f2.zhizi_k_L = (float)(0.6 * k_left[0] + 0.2 * k_left[1] + 0.2 * k_left[2]);
      }
      for (i = f2.leftguai_row; i >= 10; i--)
      {
        f1.leftline[i] = (int16)(f2.leftguai_column + f2.zhizi_k_L * (f2.leftguai_row - i));
        if (f1.leftline[i] > 159)
        {
          f1.leftline[i] = 159;
        }
        if (f1.leftline[i] < 0)
        {
          f1.leftline[i] = 0;
        }
      }

      for (i = 118; i > 10; i--)
      {
        f1.midline[i] = f1.leftline[i] + (int16)(width_real[i]) + 10; //����߲����

        if (f1.midline[i] > 159)
        {
          f1.midline[i] = 159;
        }
        if (f1.midline[i] < 0)
        {
          f1.midline[i] = 0;
        }
      }

      k_left[2] = k_left[1];
      k_left[1] = k_left[0];
    }
    if (f2.leftguaiflag == 0 && f2.rightguaiflag == 0)
    {
      for (i = 85; i > 55; i--)
      {
        if (f1.leftlineflag[i] == 1 && f1.rightlineflag[i] == 1)
        {
          m = m + 1;
        }
      }
      if (m >= 25)
      {
        f2.shiziflag[1] = 0;
      }
    }
    find_toppoint(); //����ʮ�ֺ����¶���һ���µĶ���
  }

  /******************ʮ�ֲ��߽���******************/

  /***********************************/
  //  �������
  /************************************/

  if (f2.leftturnflag != 0 || f2.rightturnflag != 0)
  {
    if (f2.leftturnflag == 1)
    {
      for (i = f2.leftturn_row; i > f2.toppoint; i--)
      {
        if (f1.rightline[i] == 159)
        {
          f1.midline[i] = f1.midline[i + 1] - 1; //���߲�������������ƫ
        }
        else
        {
          f1.midline[i] = f1.midline[i + 1] - (f1.rightline[i + 1] - f1.rightline[i]); //���߲�������������ƫ����ƫ��Ļ��ȸ��ұ߽�����ƫ��̶���ͬ
        }
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //����ʱ���ߺ�������ʧ������
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
        f1.midline[i] = f1.rightline[i] - 80; //ֱ�����ұ߽��ȥ80�������߹켣����ƫ����ȸ���
        f1.midline[i] = f1.midline[i] > 159 ? 159 : f1.midline[i];
        f1.midline[i] = f1.midline[i] < 0 ? 0 : f1.midline[i]; //�޷�
        if (f1.midline[i] <= 0)
        {
          f1.midline[i] = 0;
          L = i; //����ʱ���ߺ�������ʧ������
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
          f1.midline[i] = f1.midline[i] + 1; //���߲��֣��������ұ�ƫ
        }
        else
        {
          f1.midline[i] = f1.midline[i + 1] + (f1.leftline[i] - f1.leftline[i + 1]); //���߲�������������ƫ����ƫ��Ļ��ȸ���߽�����ƫ��̶���ͬ
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
        f1.midline[i] = f1.leftline[i] + 80; //ֱ������߽��80�������߹켣����ƫ����ȸ���
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
  column_start = f1.midline[118]; //ÿ��֮�����ʼ���趨
  column_start = column_start > 150 ? 150 : column_start;
  column_start = column_start < 10 ? 10 : column_start;
} //f2.line���յ�

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
      if (f1.leftline[i] > f1.leftline[i + 3] && f1.leftline[i] > f1.leftline[i - 3] && f1.leftline[i] > f1.leftline[i + 5] && f1.leftline[i] > f1.leftline[i - 5])
      {
        j = f1.midline[i + 5];
        if (Image[i][j] != 0 && Image[i][j - 1] != 0 && Image[i][j - 10] != 0 && Image[i - 5][j] != 0 && Image[i - 10][j] != 0 && Image[i - 15][j] != 0 && Image[i - 20][j] != 0 && Image[i - 25][j] != 0 && Image[i][j + 5] != 0)
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
  for (i = 100; i > 35; i--) //�յ�Ҫ�ڶ���֮ǰ
  {
    if (f2.lefthuanguaiflag == 0)
    {
      if (f1.leftline[i] < 80 && f1.leftline[i] > f1.leftline[i + 3] && f1.leftline[i] < f1.leftline[i - 3] && f1.leftline[i] > f1.leftline[i + 5] && f1.leftlineflag[i] == 1 && f1.leftlineflag[i + 2] == 0)
      { //i>45��Ϊ�˿��ƹյ���40�е�115����
        if (f1.leftline[i - 3] - f1.leftline[i + 3] > 12 && f1.leftlineflag[i - 2] == 1 && f1.leftlineflag[i] == 1 && f1.leftlineflag[i + 3] == 0)
        {
          if (2 * f1.leftline[i] - f1.leftline[i - 2] - f1.leftline[i + 2] > 6)
          { //Ѱ����յ�
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
      if (f1.rightline[i] > 70 && f1.rightline[i] < f1.rightline[i + 3] && f1.rightline[i] > f1.rightline[i - 3] && f1.rightline[i] < f1.rightline[i + 5] && f1.rightlineflag[i] == 1 && f1.rightlineflag[i + 2] == 0)
      { //i>45��Ϊ�˿��ƹյ���40�е�115����
        if (f1.rightline[i - 3] - f1.rightline[i + 3] < -12 && f1.rightlineflag[i - 2] == 1 && f1.rightlineflag[i + 3] == 0 && f1.rightlineflag[i] == 1)
        {
          if (f1.rightline[i + 2] + f1.rightline[i - 2] - 2 * f1.rightline[i] > 8)
          { //Ѱ���ҹյ�
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

//������С��
void find_curve1(void)
{
  int i = 0, lose_cnt;
  for (i = 115; i > f2.toppoint + 3; i--)
  {
    if (f1.leftlineflag[i] == 0)
    {
      if (f2.leftturnflag == 0 && f1.leftlineflag[i + 1] == 1 && f1.leftlineflag[i + 2] == 1 && f1.rightlineflag[i] == 1)
      { //�ҵ���߿�ʼ���ߵ���һ��
        if (f1.rightlineflag[i - 5] == 1 && i > 30)
        {
          f2.leftturn_row = i; //�Ǹ�С��
          f2.leftturnflag = 1;
          if (f2.leftturn_row < 40)
          { //�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
            if (f1.rightline[f2.leftturn_row - 5] > 100 && f1.rightline[f2.leftturn_row - 5] > f1.rightline[f2.leftturn_row])
            {
              if (f1.rightline[f2.leftturn_row - 5] - f1.rightline[f2.leftturn_row] > 10)
              {
                f2.rightturn_row = 0;
                f2.rightturnflag = 0;
              }
            }
          }
          if (f2.shiziflag[1] == 1 && f2.leftturnflag == 1) //��ֹʮ�ָ�С����жϴ�����Ӱ��
          {
            for (i = f2.leftturn_row; i > f2.toppoint; i--)
            { //�ұ߲��ܶ���̫�࣬��ֹ��ʮ�ֽ�ȥ��յ㶪ʧ�ļ�֡����
              if (f1.rightlineflag[i] == 0)
              {
                lose_cnt++;
              }
            }
            if (lose_cnt > 5)
            {
              f2.leftturn_row = 0; //�Ǹ�С��
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
          { //�����ܿ�ǰʱ��ֹ��ʮ�ֺ�Զ������ȥ��ʱ������
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
            { //�ұ߲��ܶ���̫�࣬��ֹ��ʮ�ֽ�ȥ��յ㶪ʧ�ļ�֡����
              if (f1.leftlineflag[i] == 0)
              {
                lose_cnt++;
              }
            }
            if (lose_cnt > 5)
            {
              f2.rightturn_row = 0; //�Ǹ�С��
              f2.rightturnflag = 0;
            }
          }
          break;
        }
      }
    }
  }
}

//��������
void find_curve3(void)
{
  int i = 0, flag_L = 0, flag_R = 0;
  if (f2.leftturnflag == 0 && f2.rightturnflag == 0)
  {
    for (i = 118; i > 112; i--) //����������Χ
    {
      if (f1.leftlineflag[i] == 0 && flag_L == 0) //����118-112���ж���
      {
        flag_L = 1;
      }
      if (f1.rightlineflag[i] == 0 && flag_R == 0) //����118-112���ж���
      {
        flag_R = 1;
      }
    }
    if (flag_L == 1)
    {
      for (i = 100; i > f2.toppoint + 10; i--) //100����90��������������ǰת��
      {
        if (f1.leftlineflag[i] == 1 || f1.rightlineflag[i] == 0)
        {
          flag_L = 0;
        }
      }
    }
    if (flag_R == 1)
    {
      for (i = 100; i > f2.toppoint + 10; i--) //100����90��������������ǰת��
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

//�ҹյ� ���յ���ʮ�ֵ�����
void find_guai(void)
{
  int16 i = 0;
  if (f2.toppoint < 70 && f2.toppoint > 5)
  {
    for (i = 108; i > f2.toppoint + 4; i--) //�յ�Ҫ�ڶ���֮ǰ
    {
      if (f2.leftguaiflag == 0)
      {
        if (i > 45 && f1.leftline[i] < 140 && f1.leftline[i] > f1.leftline[i + 3] && f1.leftline[i] > f1.leftline[i - 3] && f1.leftline[i] > f1.leftline[i + 5] && f1.leftline[i] > f1.leftline[i - 2] && f1.leftline[i] > f1.leftline[i - 4])
        { //i>45��Ϊ�˿��ƹյ���40�е�115����
          if (f1.leftline[i - 3] - f1.leftline[i + 3] < 2 && f1.leftlineflag[i + 3] == 1 && f1.leftlineflag[i - 5] == 0 && f1.leftlineflag[i] == 1)
          {
            if (2 * f1.leftline[i] - f1.leftline[i - 3] - f1.leftline[i + 3] > 4)
            { //Ѱ����յ�
              f2.leftguaiflag = 1;
              f2.leftguai_row = i;
              f2.leftguai_column = f1.leftline[i];
            }
          }
        }
      }
    }

    for (i = 108; i > f2.toppoint + 4; i--) //�յ�Ҫ�ڶ���֮ǰ
    {

      if (f2.rightguaiflag == 0)
      {
        if (i > 45 && f1.rightline[i] > 20 && f1.rightline[i] < f1.rightline[i + 3] && f1.rightline[i] < f1.rightline[i - 3] && f1.rightline[i] < f1.rightline[i + 5])
        { //i>45��Ϊ�˿��ƹյ���40�е�115����
          if (f1.rightline[i - 3] - f1.rightline[i + 3] > 2 && f1.rightlineflag[i + 3] == 1 && f1.rightlineflag[i - 5] == 0)
          {
            if (f1.rightline[i + 3] + f1.rightline[i - 3] - 2 * f1.rightline[i] > 4)
            { //Ѱ���ҹյ�
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

//ʮ���ж�
void shizi_judge(void)
{
  if (f2.leftguaiflag == 1)
  {
    if (f2.leftguai_column > 140)
    //����߹���ı�־Ϊ1��ǰ���£��������̫�󣬲����ٽ�����գ�Ӧ����ձ�־ȡ��
    {
      f2.leftguaiflag = 0;
      f2.leftguai_row = 0;
      f2.leftguai_column = 0;
    }
    //�����ڴ�ʱ�ұ�û�ж��ߵ�����£���ձ�־ӦΪ�㣨ʮ��ʱ�򣬲Ŵ��ڼ�����յ㣬ͬʱ����յ��Ӧ�����������ж��ұ��߶��ߣ�
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

//�ֱ��ж��Ƿ���
int8 judge_leftlose(void)
{
  int16 i = 0, count = 0, flag = 0;
  for (i = 118; i > 40; i--)
  {
    if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 1) //��߽���ڣ��ұ߽粻����
    {
      count++; //������һ
    }
  }
  if (count > 35)
  {
    flag = 1; //�����߽綪�߳�������35�У����߱�־��һ
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
    if (f1.leftlineflag[i] == 0 && f1.rightlineflag[i] == 0) //������ұ߽��Ϊ0
    {
      count++;
    }
  }
  if (count > 10) //�Ҷ��߳�������10�����϶���ʱ�Ѿ�����
  {
    flag = 1;
  }
  return flag;
}

//�жϵ�row���ұ����Ƿ���
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

//�жϵ�row��������ϱ��Ƿ���
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

//�жϵ�row�����ұ����ϱ��Ƿ���,������ҹյ�������Ƿ���ڶ���
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
    if (flag == 1) //�������ҹյ��ж��ߣ�ԭ�йյ��־���䣬���û�ж��ߣ�ԭ�йյ��־��Ϊ0
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
