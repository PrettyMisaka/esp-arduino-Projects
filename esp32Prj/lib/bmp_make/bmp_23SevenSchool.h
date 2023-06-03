#ifndef __BMP_23SEVENSCHOOL_H__
#define __BMP_23SEVENSCHOOL_H__

#include <bmp_make_ex.h>
#include <Arduino.h>

#define SS23_WIDTH  200
#define SS23_HEIGHT 300

#define SS23_MAPOFFSET 20 - 1
#define SS23_MAP_LENGTH 160

void SS23_Init();
void SS23_ReSet();
void SS23_SetisRunning(int state);
void SS23_showAngle(int _angleIndex, float _angle, int _isNeg);
void SS23_showTopNum(char _topChar);
void SS23_showRoute(char* _route);
void SS23_showInitNum(int _num);
void SS23_InitMap();
void SS23_Map_DrawArrow(int _beginIndex, int _endIndex);

#endif
