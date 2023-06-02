#ifndef __BMP_23SEVENSCHOOL_H__
#define __BMP_23SEVENSCHOOL_H__

#include <bmp_make_ex.h>

#define SS23_WIDTH  200
#define SS23_HEIGHT 300

void SS23_Init();
void SS23_SetisRunning(int state);
void SS23_showAngle(int _angleIndex, float _angle);
void SS23_showTopNum(char _topChar);
void SS23_showRoute(String _route);
void SS23_showInitNum(int _num);

#endif
