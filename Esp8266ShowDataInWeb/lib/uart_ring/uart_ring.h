#ifndef __UART_RING_H__
#define __UART_RING_H__

#define UART_RING_VERSION 1.1f
#define FLOAT_FLAG "{FLOAT}"
#define INT_FLAG "{INT}"
#define CMD_STRING_LENGTH 30
#define RING_BUFF_LENGTH 64
#define CMD_BUFF_LENGTH 40
// #define RING_USE_LINK_LIST 
 #define RING_USE_ARR

typedef unsigned int uint16_ring;
typedef unsigned char uint8_ring;
typedef float float_ring;

typedef enum{
    RING_OK = 0,
    RING_ERR,
    RING_WAIT,
    RING_FREEHEAD,
    RING_INT,
    RING_FLOAT,
    RING_WAITFORFLAG,
    RING_BUFF_OVERFLOW
}ring_state;
#ifndef RING_USE_ARR
    typedef struct ringNodeInf{
        uint8_ring              data;
        struct ringNodeInf *next;
    }ringNodeTypedef;
#endif

typedef struct {
    ring_state state;
    int offset;
    int intData;
    float_ring floatData;
} callBackFun_EventTypedef;

typedef struct cmdNodeInf{
    uint8_ring  cmd_String[CMD_STRING_LENGTH];
    void (*callBackFn)(callBackFun_EventTypedef e);
#ifndef RING_USE_ARR
    struct cmdNodeInf * next;
#endif
} cmdNodeTypedef;

typedef struct {
#ifndef RING_USE_ARR
    ringNodeTypedef *ringHeadP;
    ringNodeTypedef *ringTailP;  
#else
    uint8_ring ringBuffArr[RING_BUFF_LENGTH];
    int ringBuffHead;
    int ringBuffTail;
#endif

#ifndef RING_USE_ARR
    cmdNodeTypedef *cmdHeadP;
    cmdNodeTypedef *cmdTailP;
#else
    cmdNodeTypedef cmdBuffArr[CMD_BUFF_LENGTH];
    int cmdBuffLength;
#endif
} uartRingParameterTypedef;

/*----------INIT FUN----------*/
void ringBuff_Total_Init(uartRingParameterTypedef * uart_ring_param_P);
void ringBuff_Init(uartRingParameterTypedef * uart_ring_param_P);
void cmdBuff_Init(uartRingParameterTypedef * uart_ring_param_P);
/*----------PUSH AND DEL FUN----------*/
void ringBuff_Push( uartRingParameterTypedef * uart_ring_param_P, uint8_ring data);
ring_state cmdBuff_Push( uartRingParameterTypedef * uart_ring_param_P,
                         uint8_ring cmd_String[CMD_STRING_LENGTH],
                         void (*callBackFn)(callBackFun_EventTypedef e));
ring_state cmdBuff_DelCmd( uartRingParameterTypedef * uart_ring_param_P,
                           uint8_ring cmd_String[CMD_STRING_LENGTH]);
/*----------HANDLE FUN---------*/
ring_state ringShowBuff( uartRingParameterTypedef * uart_ring_param_P, char ringBuffStr[RING_BUFF_LENGTH]);
ring_state ringBuffHandleFun( uartRingParameterTypedef * uart_ring_param_P);


#endif
