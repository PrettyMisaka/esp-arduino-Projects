#include <bmp_23SevenSchool.h>

void SS23_Init(){
    bmpBase.clear(WHITE);
    SS23_SetisRunning(0);
    SS23_showAngle(0,0.0);
    SS23_showAngle(1,0.0);
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*3, BLACK, WHITE, 16, "TOP:");
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*4, BLACK, WHITE, 16, "route:");
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*5, BLACK, WHITE, 16, "Starting number:");
}

void SS23_SetisRunning(int state){
    String _state;
    if(state) _state = "ON ";
    else _state = "OFF";
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16, BLACK, WHITE, 16, "STATE:%s", _state);
}

static float angleA, angleB;
void SS23_showAngle(int _angleIndex, float _angle){
    if(_angleIndex == 0){
        angleA = _angle;
    }
    else if(_angleIndex == 1){
        angleB = _angle;
    }
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*2, BLACK, WHITE, 16, "A:%3.3f B:%3.3f", angleA, angleB);
}

void SS23_showTopNum(char _topChar){
    if(!(('a'<=_topChar&&_topChar<='f')||('A'<=_topChar&&_topChar<='F'))) return;
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*3, BLACK, WHITE, 16, "TOP:%c", _topChar);
}

void SS23_showRoute(String _route){
    bmpBase.clear( 0, SS23_HEIGHT - 16*4, 200, SS23_HEIGHT - 16*5, WHITE);
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*4, BLACK, WHITE, 16, "route:%s", _route);
}

void SS23_showInitNum(int _num){
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*5, BLACK, WHITE, 16, "Starting number:%2d", _num);
}
