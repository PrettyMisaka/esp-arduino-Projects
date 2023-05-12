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

