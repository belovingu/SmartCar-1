#ifndef __IMAGE_PROCESS
#define __IMAGE_PROCESS
#include "headfile.h"

#define img_width MT9V03X_CSI_W
#define img_height MT9V03X_CSI_H

#define white_point 255
#define black_point 0

extern int left_line[img_height];
extern int right_line[img_height];
extern int mid_line[img_height];
extern int road_width[img_height];

extern int line_end_pos_max;
extern int drop_line_num;
extern int cross_road_flag;
extern int road_end_pos;

extern int left_line_end_pos;
extern int mid_line_end_pos;
extern int right_line_end_pos;
extern int midright_line_end_pos;
extern int midleft_line_end_pos;
extern int leftleft_line_end_pos;
extern int rightright_line_end_pos;

extern uint8_t ImageThreshold;
extern uint8_t BinImage[MT9V03X_CSI_H][MT9V03X_CSI_W];
extern uint8_t FilterImage[MT9V03X_CSI_H][MT9V03X_CSI_W];
extern uint8_t compressedImage[img_width * img_height / 8];

int LQMeanThreshold(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W]);
int GetOSTUThreshold(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W]);

void MeanFilter(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W], uint8_t img_out[MT9V03X_CSI_H][MT9V03X_CSI_W]);
void Convert2Binary(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W], uint8_t img_out[MT9V03X_CSI_H][MT9V03X_CSI_W], uint8_t mode);

void ResetVarialble();
void CrossRoadProcess(const uint8_t img[img_height][img_width]);
void FindMidLine(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W]);
void CrossRoadProcess(const uint8_t img[MT9V03X_CSI_H][MT9V03X_CSI_W]);
void LoopRoadDetect();

int StopCarJudge(const uint8_t img[img_height][img_width]);
void Least3Error();

#endif