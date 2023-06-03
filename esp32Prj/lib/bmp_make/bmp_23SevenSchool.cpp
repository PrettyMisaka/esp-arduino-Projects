#include <bmp_23SevenSchool.h>

static int _SS23_state = 0;
static int _SS23_route_cnt = 0;
void SS23_Init(){
    SS23_ReSet();
    SS23_SetisRunning(0);
}

void SS23_ReSet(){
    bmpBase.clear(WHITE);
    SS23_showAngle(0,0.0,0);
    SS23_showAngle(1,0.0,0);
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*3, BLACK, WHITE, 16, "TOP:");
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*4, BLACK, WHITE, 16, "route:");
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*5, BLACK, WHITE, 16, "Starting number:");
}

void SS23_SetisRunning(int state){
    _SS23_state = state;
    if(_SS23_state)
        bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16, BLACK, WHITE, 16, "STATE:ON ");
    else{
        // SS23_ReSet();
        bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16, BLACK, WHITE, 16, "STATE:OFF");
    }
}

static float angleA = 0.0, angleB = 0.0;
void SS23_showAngle(int _angleIndex, float _angle, int _isNeg){
    if(_isNeg) _angle = -_angle;
    if(_angleIndex == 0){
        angleA = _angle;
    }
    else if(_angleIndex == 1){
        angleB = _angle;
    }
    bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*2, BLACK, WHITE, 16, "A:%3.3f B:%3.3f", angleA, angleB);
    // if(!_SS23_state)
    //     bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*2, BLACK, WHITE, 16, "A:0.000 B:0.000");
}

void SS23_showTopNum(char _topChar){
    if(!(('a'<=_topChar&&_topChar<='f')||('A'<=_topChar&&_topChar<='F'))) return;
        bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*3, BLACK, WHITE, 16, "TOP:%c", _topChar);
    // if(!_SS23_state)
    //     bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*3, BLACK, WHITE, 16, "TOP:  ");
}

void SS23_showRoute(char * _route){
    bmpBase.clear( 0, SS23_HEIGHT - 16*4, 200, SS23_HEIGHT - 16*5, WHITE);
        bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*4, BLACK, WHITE, 16, "route:%s", _route);
    // if(!_SS23_state)
    //     bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*4, BLACK, WHITE, 16, "route:", _route);
}

void SS23_showInitNum(int _num){
        bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*5, BLACK, WHITE, 16, "Starting number:%2d", _num);
    // if(!_SS23_state)
    //     bmpBase.printf_bmpString( 0, SS23_HEIGHT - 16*5, BLACK, WHITE, 16, "Starting number:");
}

static unsigned char _red = 255, _green = 255, _blue = 255;
void SS23_InitMap(){
    bmpBase.clear(  SS23_MAPOFFSET, SS23_MAPOFFSET, SS23_MAPOFFSET + SS23_MAP_LENGTH, SS23_MAPOFFSET + SS23_MAP_LENGTH, WHITE);
    bmpBase.drawRectangle(  SS23_MAPOFFSET, SS23_MAPOFFSET, SS23_MAPOFFSET + SS23_MAP_LENGTH, SS23_MAPOFFSET + SS23_MAP_LENGTH, BLACK);
    for (int i = 1; i < 4; i++){
        bmpBase.drawLine( SS23_MAPOFFSET, SS23_MAPOFFSET + i*SS23_MAP_LENGTH/4, SS23_MAPOFFSET + SS23_MAP_LENGTH, SS23_MAPOFFSET + i*SS23_MAP_LENGTH/4, BLACK);
        bmpBase.drawLine( SS23_MAPOFFSET + i*SS23_MAP_LENGTH/4, SS23_MAPOFFSET, SS23_MAPOFFSET + i*SS23_MAP_LENGTH/4, SS23_MAPOFFSET + SS23_MAP_LENGTH, BLACK);
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            bmpBase.printf_bmpString( SS23_MAPOFFSET + i*SS23_MAP_LENGTH/4 + 19, SS23_MAPOFFSET + j*SS23_MAP_LENGTH/4 + 19, BLACK, WHITE, 16, "%d", i+4*j);
        }
    }
    _SS23_route_cnt = 0;
    _red = 255;
    _green = 255;
    _blue = 255;
}

void SS23_Map_DrawArrow(int _beginIndex, int _endIndex){
    // int _errorVal = _endIndex - _beginIndex;
    // int _abs_error = _errorVal;
    // if(_abs_error < 0)
    //     _abs_error = -_abs_error;
    // if(!(_abs_error == 4 || _abs_error == 1)) return;
    // uint16_t _arrow_color;
    // switch (_SS23_route_cnt%3){
    //     case 0:
    //         _red -= 20*(_SS23_route_cnt/3);
    //         _arrow_color = (((_red)&0xF8)<<8)|(((0)&0xFC)<<3)|(((0)&0xF8)>>3);
    //         break;
    //     case 1:
    //         _green -= 20*(_SS23_route_cnt/3);
    //         _arrow_color = (((0)&0xF8)<<8)|(((_green)&0xFC)<<3)|(((0)&0xF8)>>3);
    //         break;
    //     case 2:
    //         _blue -= 20*(_SS23_route_cnt/3);
    //         _arrow_color = (((0)&0xF8)<<8)|(((0)&0xFC)<<3)|(((_blue)&0xF8)>>3);
    //         break;
    //     default:
    //         break;
    // }
    // _SS23_route_cnt++;
    // uint16_t _arrow_color = (((_red)&0xF8)<<8)|(((_green)&0xFC)<<3)|(((_blue)&0xF8)>>3);
    // int _offset = SS23_MAPOFFSET + 19;
    int _offset = 39;
    // int _length = SS23_MAP_LENGTH/4;
    int _length = 40;
    int x0 = _offset  + ((_beginIndex % 4) * _length);
    int y0 = _offset + ((_beginIndex / 4) * _length);
    int x1 = _offset + ((_endIndex % 4) *_length);
    int y1 = _offset + ((_endIndex / 4) * _length);
    bmpBase.drawLine( x0, y0, x1, y1, RED);
}