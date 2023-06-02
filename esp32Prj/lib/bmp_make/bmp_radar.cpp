#include "bmp_radar.h"
#include "math.h"

int radarMaxValue = 1500;
int radarMaxPixVal = 150;
int radarDataOffset[2][181];

void Radar_Init(){
    for(int i = 0; i <180; i++){
        radarDataOffset[0][i] = 0;
        radarDataOffset[1][i] = 0;
    }
}

void Radar_Push(int val, int angle){
    if(!(0 <= angle && angle <= 180)) return;
    if(angle >= 90) angle = 180 - angle;
    double val2Pix = val*(float)radarMaxPixVal/radarMaxValue;
    float sinVal = sin((angle/180.0)*PI);
    float cosVal = cos((angle/180.0)*PI);
    if(angle == 0){
        radarDataOffset[0][angle] = val2Pix;
        radarDataOffset[1][angle] = 0;
        }
    else if (angle == 180){
        radarDataOffset[0][angle] = -val2Pix;
        radarDataOffset[1][angle] = 0;
        }
    else if(angle == 90){
        radarDataOffset[0][angle] = 0;
        radarDataOffset[1][angle] = val2Pix;
        }
    else{
        radarDataOffset[0][angle] = (int)val2Pix*cosVal;//offsetX
        radarDataOffset[1][angle] = (int)val2Pix*sinVal;//offsetY
        }
}

void Rader_Push_Index(int index, int xOffset, int YOffset){// xOffset 0~300
    if(!(0<=index&&index<=180)) return;
    radarDataOffset[0][index] = xOffset;//offsetX
    radarDataOffset[1][index] = YOffset;//offsetY
}

void Radar_flash(){
    bmpBase.clear(WHITE);
    int tmp = 50;
    for(int i = 1; i < 4; i++){
        bmpBase.drawCircle(150 , 0, tmp*i, BLACK); 
    }
    for(int i = 0; i <180; i++){
        if(radarDataOffset[0][i] == 0 && radarDataOffset[1][i] == 0) 
            continue;
        else{
            bmpBase.drawPixel(150+radarDataOffset[0][i],radarDataOffset[1][i],RED);
            if(i!=0)
            bmpBase.drawLine(150+radarDataOffset[0][i-1],radarDataOffset[1][i-1],150+radarDataOffset[0][i],radarDataOffset[1][i],RED);

            // for( int j = i; j >= 1; j++){
            //     if(!(radarDataOffset[0][j -1] == 0 && radarDataOffset[1][j - 1] == 0))
            //         bmpBase.drawLine(150+radarDataOffset[0][j-1],radarDataOffset[1][j-1],150+radarDataOffset[0][i],radarDataOffset[1][i],RED);
            // }
        }
    }
}
