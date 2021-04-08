#ifndef __UTIL
#define __UTIL
#include "headfile.h"

//0: csharp
//1: vofa
// #define DEBUG_PROTOCOL_CSHARP 1
#define DEBUG_PROTOCOL_VOFA 0
extern uint8_t debug_protocol;
#define ABS(x) (x > 0 ? x : (-x))

int str2bool(char *str);
double myatof(const char *str);

float limit_ab_float(float x, float a, float b);
int limit_ab_int(int x, int a, int b);

float limit_float(float x, float y);
int limit_int(int x, int y);

void upload_zf_gray();
void upload_zf_bin();

#endif // __UTIL
