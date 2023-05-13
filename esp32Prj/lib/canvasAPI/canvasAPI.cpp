#include <canvasAPI.h>

CanvasAPI canvasAPI_draw;
CanvasAPI::CanvasAPI(){
    canvasCmdCode = "";
}
void CanvasAPI::canvasCmdPush(String cmdString){
    canvasCmdCode += cmdString;
}
void CanvasAPI::clearCanvas(){
    canvasCmdCode = "";
}
void CanvasAPI::showCanvasCmd(){
    Serial.println(canvasCmdCode);
}
void CanvasAPI::setFillColor(const char* color){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillStyle=\"%s\";",color);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::setStrokeColor(const char* color){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.strokeStyle=\"%s\";",color);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::fillRect(int x, int y, int width, int height){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillRect(%d,%d,%d,%d);",x,y,width,height);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::strokeRect(int x, int y, int width, int height){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.strokeRect(%d,%d,%d,%d);",x,y,width,height);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::clearRect(int x, int y, int width, int height){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.clearRect(%d,%d,%d,%d);",x,y,width,height);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::drawLine(int x_begin, int y_begin, int x_end, int y_end){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.moveTo(%d, %d);ctx.lineTo(%d, %d);ctx.stroke();",x_begin,y_begin,x_end,y_end);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::setFont(const char* size_px,const char* font){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.font=\"%s %s\";",size_px,font);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};
void CanvasAPI::drawStr(int x, int y,const char* str){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillText(\"%s\",%d,%d);",str,x,y);
    String tmpString = str_tmp;
    canvasCmdPush(tmpString);
};

CoordinateAPI CoordinateAPI_Coordinate1(10,310);
CoordinateAPI::CoordinateAPI(int x, int y){
    length = 0;
    sprintf(color,"#000000");
    zero_x = x;
    zero_y = y;
}
void CoordinateAPI::setZeroPoint(int x, int y){
    zero_x = x;
    zero_y = y;
};
void CoordinateAPI::setLineColor(const char* color_str){
    sprintf(color,"%s",color_str);
}
int CoordinateAPI::pushArrayValue(int x, int y){
    int isPush = 0;
    if(length == CANVAS_COORDINATE_ARR_LENGTH)
        return 0;
    if(length == 0){
        array[0][0] = x;
        array[1][0] = y;
        isPush++;
    }
    else{
        for (int i = 0; i < length; i++){
            if(x < array[0][i] && i == 0){
                array[0][1] = array[0][0];
                array[1][1] = array[1][0];
                array[0][0] = x;
                array[1][0] = y;
                isPush++;
                break;
            }
            else if(x > array[0][i] && i == length - 1){
                array[0][length] = x;
                array[1][length] = y;
                isPush++;
                break;
            }
            else if((array[0][i] < x && x <= array[0][i+1])&&( 0 <= i && i < length - 1)){
                if(array[1][i+1] == y) return 0;
                for (int index = length; index > i + 1; index--){
                    array[0][index] = array[0][index - 1];
                    array[1][index] = array[1][index - 1];
                }
                array[0][i+1] = x;
                array[1][i+1] = y;
                isPush++;
                break;
            }
        }
    }
    if(isPush){
        length++;
        return 1;
    }
    else{
        return 0;
    }
}
int CoordinateAPI::delArrayValue(int x,int y){
    if(length == 0) return 0;
    for (int i = 0; i < length; i++){
        if( array[0][i] == x && array[1][i] == y){
            length--;
            for (int index = i; index < length; index++){
                array[0][index] = array[0][index + 1];
                array[1][index] = array[1][index + 1];
            }
            return 1;
        }
    }
    return 0;
}
void CoordinateAPI::clearArrayValue(){
    length = 0;
}
void CoordinateAPI::printArray(){
    String string_x;
    String string_y;
    char string_char_tmp[10];
    if(length == 0) Serial.println("NULL");
    for (int i = 0; i < length; i++){
        sprintf(string_char_tmp,"%d|",array[0][i]);
        string_x += string_char_tmp;
        sprintf(string_char_tmp,"%d|",array[1][i]);
        string_y += string_char_tmp;
    }
    Serial.println(string_x);
    Serial.println(string_y);
}
void CoordinateAPI::showCoordinate(){
    clearCanvas();
    setStrokeColor("#000000");
    drawLine( zero_x, zero_y, zero_x + 400, zero_y);

    drawLine( zero_x + 400, zero_y, zero_x + 400 - 5, zero_y - 5);
    drawLine( zero_x + 400, zero_y, zero_x + 400 - 5, zero_y + 5);
    
    drawLine( zero_x, zero_y, zero_x, zero_y-400);

    drawLine( zero_x, zero_y-400, zero_x+5, zero_y-400+5);
    drawLine( zero_x, zero_y-400, zero_x-5, zero_y-400+5);
    
    setStrokeColor(color);
    if(length != 0){
        drawLine( zero_x, zero_y, zero_x + array[0][0], zero_y-array[1][0]);
        for (int i = 0; i < length; i++){
            if( i < length - 1){
                drawLine( zero_x + array[0][i], zero_y-array[1][i], zero_x + array[0][i+1], zero_y-array[1][i+1]);
            }
        }
    }
}
