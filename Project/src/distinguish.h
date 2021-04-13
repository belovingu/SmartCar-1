#ifndef _PINGDING_H
#define _PINGDING_H
#include "common.h"

extern int16 foresight;

extern int16 s_zhi;
extern int16 s_max;
extern int16 s_wan1;
extern int16 s_wan2;
extern int16 s_cross;

extern int16 stop_car[20];
extern int16 stop_car_flag;
extern int16 road_type[200];
extern int16 shortstraight_flag;
extern int16 long_straight_flag;

extern int16 steer_error[30];
extern int16 speed_set;
extern int16 steer_error_ave;

void long_straight_judge();
void short_straight_judge();
void cross_straight_judge();
void stop_car_judge();
void compute_error();
void velocity_set();

#endif