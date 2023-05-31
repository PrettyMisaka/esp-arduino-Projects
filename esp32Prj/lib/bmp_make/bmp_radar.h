#ifndef __BMP_RADAR_H__
#define __BMP_RADAR_H__

#include <bmp_make_ex.h>

#ifndef PI
#define PI 3.1415926535898
#endif

void Radar_Init();
void Radar_Push(int val, short angle);
void Radar_flash();

#endif
