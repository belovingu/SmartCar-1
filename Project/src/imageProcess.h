#ifndef __IMAGE_PROCESS
#define __IMAGE_PROCESS
#include "headfile.h"
#define img_width MT9V03X_CSI_W
#define img_height MT9V03X_CSI_H

extern uint8_t ImageThreshold;
extern uint8_t BinImage[img_height][img_width];
extern uint8_t FilterImage[img_height][img_width];
extern uint8_t compressedImage[img_width * img_height / 8];

int LQMeanThreshold(const uint8_t img[img_height][img_width]);
int GetOSTUThreshold(const uint8_t img[img_height][img_width]);

void gray_mean_filter(const uint8_t img[img_height][img_width], uint8_t img_out[img_height][img_width]);
void gray_to_binary(const uint8_t img[img_height][img_width], uint8_t img_out[img_height][img_width], uint8_t mode);

#endif