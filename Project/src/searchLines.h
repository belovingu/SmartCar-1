#ifndef __SEARCH_LINES_H__
#define __SEARCH_LINES_H__
#include "headfile.h"
typedef struct
{
    int16 midline[120];
    int16 leftline[120];
    int16 rightline[120];
    int16 leftlineflag[120];
    int16 rightlineflag[120];

} Findline1_TypeDef;

typedef struct
{
    int16 leftturnflag;
    int16 rightturnflag;
    int16 leftturn_row;
    int16 rightturn_row;
    int16 leftguai_column;
    int16 leftguai_row;
    int16 leftguaiflag;
    int16 rightguaiflag;
    int16 rightguai_column;
    int16 rightguai_row;
    int16 leftguaiflag_up;
    int16 leftguai_column_up;
    int16 leftguai_row_up;
    int16 rightguaiflag_up;
    int16 rightguai_column_up;
    int16 rightguai_row_up;
    int16 toppoint;
    int16 toppoint_flag;
    int16 shiziflag[2];
    int16 huan;
    int16 huanrightflag;
    int16 huanleftflag;
    int16 huanleft_row;
    int16 huanleft_column;
    int16 huanright_row;
    int16 huanright_column;
    int16 lefthuanguai_row;
    int16 lefthuanguai_column;
    int16 lefthuanguaiflag;
    int16 righthuanguai_row;
    int16 righthuanguai_column;
    int16 righthuanguaiflag;
    float zhizi_k_L;
    float zhizi_k_R;
} Findline2_TypeDef;

extern Findline1_TypeDef f1;
extern Findline2_TypeDef f2;
extern int16 width[120];
extern int16 width_real[120];
extern int16 text_width[120];
extern int16 column_start;

void bianxian(void);
void find_toppoint(void);
void find_curve1(void);
void find_curve3(void);
void find_guai(void);
void shizi_judge(void);
void find_huan1(void);
void find_huan2(void);
void find_huan3(void);
void find_huanguai(void);
void findline1(void);
void findline2(void);
int8 judge_leftlose(void);
int8 judge_rightlose(void);
int8 judge_bothlose(void);
int8 right(int16 row);
int8 left(int16 row);

#endif