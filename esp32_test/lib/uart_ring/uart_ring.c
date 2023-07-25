#include "uart_ring.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**/
static callBackFun_EventTypedef ringBuffCmpCmd(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    cmdNodeTypedef *cmdNodeP
#else
    int cmdIndex
#endif
);
static callBackFun_EventTypedef intFlagJudge(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    ringNodeTypedef *ringNodeP, cmdNodeTypedef *cmdNodeP,
#else
    int ringNodeP, int cmdIndex,
#endif
    int index);
static callBackFun_EventTypedef floatFlagJudge(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    ringNodeTypedef *ringNodeP, cmdNodeTypedef *cmdNodeP,
#else
    int ringNodeP, int cmdIndex,
#endif
    int index);

/* total init*/
void ringBuff_Total_Init(uartRingParameterTypedef *uart_ring_param_P)
{
    ringBuff_Init(uart_ring_param_P);
    cmdBuff_Init(uart_ring_param_P);
}

/*----------RING BUFF FUN----------*/
void ringBuff_Init(uartRingParameterTypedef *uart_ring_param_P)
{
#ifndef RING_USE_ARR
    uart_ring_param_P->ringHeadP = NULL;
    uart_ring_param_P->ringTailP = uart_ring_param_P->ringHeadP;
#else
    uart_ring_param_P->ringBuffHead = 0;
    uart_ring_param_P->ringBuffTail = uart_ring_param_P->ringBuffHead - 1;
#endif
}

void ringBuff_Push(uartRingParameterTypedef *uart_ring_param_P, uint8_ring data)
{
#ifndef RING_USE_ARR
    if (uart_ring_param_P->ringHeadP == NULL)
    {
        uart_ring_param_P->ringHeadP = (ringNodeTypedef *)malloc(sizeof(ringNodeTypedef));
        uart_ring_param_P->ringTailP = uart_ring_param_P->ringHeadP;
    }
    else
    {
        uart_ring_param_P->ringTailP->next = (ringNodeTypedef *)malloc(sizeof(ringNodeTypedef));
        uart_ring_param_P->ringTailP = uart_ring_param_P->ringTailP->next;
    }
    uart_ring_param_P->ringTailP->data = data;
    uart_ring_param_P->ringTailP->next = NULL;
#else
    uart_ring_param_P->ringBuffTail = (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH;
    uart_ring_param_P->ringBuffArr[uart_ring_param_P->ringBuffTail] = data;
#endif
}

ring_state ringShowBuff(uartRingParameterTypedef *uart_ring_param_P, char ringBuffStr[RING_BUFF_LENGTH])
{
#ifndef RING_USE_ARR
    ringNodeTypedef *tmpP = uart_ring_param_P->ringHeadP;
    if (tmpP == NULL)
#else
    if (uart_ring_param_P->ringBuffHead == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
    {
        sprintf(ringBuffStr, "");
        return RING_WAIT;
    }
#ifndef RING_USE_ARR
    for (int i = 0; i < RING_BUFF_LENGTH; i++)
    {
        ringBuffStr[i] = tmpP->data;
        if (tmpP->next == NULL)
        {
            if (i != RING_BUFF_LENGTH - 1)
                ringBuffStr[i + 1] = 0;
            return RING_OK;
        }
        tmpP = tmpP->next;
    }
#else
    int tmp = uart_ring_param_P->ringBuffHead - uart_ring_param_P->ringBuffTail;
    if (tmp < 0)
        tmp += RING_BUFF_LENGTH;
    for (int i = 0; i < tmp + 1; i++)
    {
        ringBuffStr[i] = uart_ring_param_P->ringBuffArr[(i + uart_ring_param_P->ringBuffHead) % RING_BUFF_LENGTH];
    }
#endif
    return RING_ERR;
}

static ring_state ringBuff_FreeHeadP(uartRingParameterTypedef *uart_ring_param_P)
{
#ifndef RING_USE_ARR
    if (uart_ring_param_P->ringHeadP == NULL)
        return RING_ERR;
    ringNodeTypedef *tmpP = uart_ring_param_P->ringHeadP;
    uart_ring_param_P->ringHeadP = uart_ring_param_P->ringHeadP->next;
    free(tmpP);
#else
    if (uart_ring_param_P->ringBuffHead == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
        return RING_WAIT;
    uart_ring_param_P->ringBuffHead = (uart_ring_param_P->ringBuffHead + 1) % RING_BUFF_LENGTH;
#endif
    return RING_OK;
}

/*----------CMD Fun----------*/

void cmdBuff_Init(uartRingParameterTypedef *uart_ring_param_P)
{
#ifndef RING_USE_ARR
    uart_ring_param_P->cmdHeadP = NULL;
    uart_ring_param_P->cmdTailP = uart_ring_param_P->cmdHeadP;
#else
    uart_ring_param_P->cmdBuffLength = 0;
#endif
}

ring_state cmdBuff_Push(uartRingParameterTypedef *uart_ring_param_P, uint8_ring cmd_String[CMD_STRING_LENGTH], void (*callBackFn)(callBackFun_EventTypedef e))
{
    if (strcmp("", (char *)cmd_String) == 0)
        return RING_ERR;
#ifndef RING_USE_ARR
    if (uart_ring_param_P->cmdHeadP != NULL)
    {
        cmdNodeTypedef *tmpP = uart_ring_param_P->cmdHeadP;
        while (1)
        {
            if (strcmp((char *)tmpP->cmd_String, (char *)cmd_String) == 0)
                return RING_ERR;
            if (tmpP->next == NULL)
                break;
            tmpP = tmpP->next;
        }
    }
    if (uart_ring_param_P->cmdHeadP == NULL)
    {
        uart_ring_param_P->cmdHeadP = (cmdNodeTypedef *)malloc(sizeof(cmdNodeTypedef));
        uart_ring_param_P->cmdTailP = uart_ring_param_P->cmdHeadP;
    }
    else
    {
        uart_ring_param_P->cmdTailP->next = (cmdNodeTypedef *)malloc(sizeof(cmdNodeTypedef));
        uart_ring_param_P->cmdTailP = uart_ring_param_P->cmdTailP->next;
    }
    sprintf(uart_ring_param_P->cmdTailP->cmd_String, cmd_String);
    uart_ring_param_P->cmdTailP->callBackFn = callBackFn;
    uart_ring_param_P->cmdTailP->next = NULL;
#else
    if (uart_ring_param_P->cmdBuffLength != 0)
    {
        for (int i = 0; i < uart_ring_param_P->cmdBuffLength; i++)
        {
            if (strcmp((char *)uart_ring_param_P->cmdBuffArr[i].cmd_String, (char *)cmd_String) == 0)
                return RING_ERR;
            if (i == CMD_BUFF_LENGTH - 1)
                return RING_BUFF_OVERFLOW;
        }
    }
    sprintf((char *)uart_ring_param_P->cmdBuffArr[uart_ring_param_P->cmdBuffLength].cmd_String, (char *)cmd_String);
    uart_ring_param_P->cmdBuffArr[uart_ring_param_P->cmdBuffLength].callBackFn = callBackFn;
    uart_ring_param_P->cmdBuffLength++;
#endif
    return RING_OK;
}

ring_state cmdBuff_DelCmd(uartRingParameterTypedef *uart_ring_param_P, uint8_ring cmd_String[CMD_STRING_LENGTH])
{
#ifndef RING_USE_ARR
    cmdNodeTypedef *tmpP = uart_ring_param_P->cmdHeadP, *preNode;
    while (1)
    {
        if (strcmp((char *)uart_ring_param_P->cmdHeadP->cmd_String, (char *)cmd_String) == 0)
        {
            if (tmpP == uart_ring_param_P->cmdHeadP)
                uart_ring_param_P->cmdHeadP = NULL;
            else
                preNode->next = tmpP->next;
            free(tmpP);
            return RING_OK;
        }
        else if (tmpP->next == NULL)
            return RING_ERR;
        else
        {
            preNode = tmpP;
            tmpP = tmpP->next;
        }
    }
#else
    int index = 0;
    if (uart_ring_param_P->cmdBuffLength == 0)
        return RING_WAIT;
    for (int i = 0; i < uart_ring_param_P->cmdBuffLength; i++)
    {
        if (strcmp((char *)uart_ring_param_P->cmdBuffArr[i].cmd_String, (char *)cmd_String) == 0)
        {
            index = i;
            uart_ring_param_P->cmdBuffLength--;
            for (int i = index; i < uart_ring_param_P->cmdBuffLength; i++)
            {
                uart_ring_param_P->cmdBuffArr[i] = uart_ring_param_P->cmdBuffArr[i + 1];
            }
            return RING_OK;
        }
    }
    return RING_ERR;
#endif
}

/*----------CMP CMD AND CALL CALLBACK FUN----------*/
ring_state ringBuffHandleFun(uartRingParameterTypedef *uart_ring_param_P)
{
#ifndef RING_USE_ARR
    cmdNodeTypedef *tmp_cmdP = uart_ring_param_P->cmdHeadP;
    if (tmp_cmdP == NULL)
        return RING_ERR;
    if (uart_ring_param_P->ringHeadP == NULL)
        return RING_WAIT;
#else
    if (uart_ring_param_P->cmdBuffLength == 0)
        return RING_ERR;
    if (uart_ring_param_P->ringBuffHead == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
        return RING_WAIT;
    int tmp_cmdP = 0;
#endif
    callBackFun_EventTypedef e;
    while (1)
    {
        e = ringBuffCmpCmd(uart_ring_param_P, tmp_cmdP);
        switch (e.state)
        {
        case RING_ERR:
#ifndef RING_USE_ARR
            if (tmp_cmdP->next == NULL)
#else
            if (tmp_cmdP == uart_ring_param_P->cmdBuffLength - 1)
#endif
            {
                ringBuff_FreeHeadP(uart_ring_param_P);
                return RING_FREEHEAD;
            }
#ifndef RING_USE_ARR
            else
                tmp_cmdP = tmp_cmdP->next;
#else
            else
                tmp_cmdP++;
#endif
            break;

        case RING_WAIT:
            return RING_WAIT;

        case RING_WAITFORFLAG:
            return RING_WAITFORFLAG;

        case RING_OK:
        case RING_INT:
        case RING_FLOAT:
            for (int i = 0; i < e.offset; i++)
                ringBuff_FreeHeadP(uart_ring_param_P);
#ifndef RING_USE_ARR
            tmp_cmdP->callBackFn(e);
#else
            uart_ring_param_P->cmdBuffArr[tmp_cmdP].callBackFn(e);
#endif
            return e.state;

        default:
            return RING_ERR;
        }
    }
}

static callBackFun_EventTypedef ringBuffCmpCmd(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    cmdNodeTypedef *cmdNodeP
#else
    int cmdIndex
#endif
)
{
#ifndef RING_USE_ARR
    ringNodeTypedef *tmp_ringP = uart_ring_param_P->ringHeadP;
    cmdNodeTypedef *tmp_cmdP = cmdNodeP;
#else
    int tmp_ringP = uart_ring_param_P->ringBuffHead;
#endif
    callBackFun_EventTypedef event;
    int isDelRingHeadP = 0;
    for (
        int i = 0; i < CMD_STRING_LENGTH; i++)
    {
#ifndef RING_USE_ARR
        if (tmp_cmdP->cmd_String[i] == 0)
#else
        if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[i] == 0)
#endif
        {
            switch (event.state)
            {
            case RING_INT:
                event.offset = event.offset - 4 + i;
                break;
            case RING_FLOAT:
                event.offset = event.offset - 6 + i;
                break;
            default:
                event.state = RING_OK;
                event.offset = i;
                break;
            }
            return event;
        }
#ifndef RING_USE_ARR
        if (tmp_ringP->data == tmp_cmdP->cmd_String[i])
#else
        if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[i] == uart_ring_param_P->ringBuffArr[tmp_ringP])
#endif
        {
            isDelRingHeadP++;
#ifndef RING_USE_ARR
            event = intFlagJudge(uart_ring_param_P, tmp_ringP, tmp_cmdP, i);
#else
            event = intFlagJudge(uart_ring_param_P, tmp_ringP, cmdIndex, i);
#endif
            if (event.state == RING_ERR)
            {
                return event;
            }
            else if (event.state == RING_WAIT || event.state == RING_WAITFORFLAG)
            {
#ifndef RING_USE_ARR
                if (tmp_cmdP->cmd_String[i + 1] == 0)
#else
                if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[i + 1] == 0)
#endif
                {
                    event.state = RING_OK;
                    event.offset = i;
                    return event;
                }
#ifndef RING_USE_ARR
                if (tmp_ringP->next == NULL || event.state == RING_WAITFORFLAG)
                    return event;
                tmp_ringP = tmp_ringP->next;
#else
                if (tmp_ringP == uart_ring_param_P->ringBuffTail || event.state == RING_WAITFORFLAG)
                    return event;
                tmp_ringP = (tmp_ringP + 1) % RING_BUFF_LENGTH;
#endif
                continue;
            }
            else if (event.state == RING_INT || event.state == RING_FLOAT)
            {
                if (event.state == RING_INT)
                    i = i + 4;
                else if (event.state == RING_FLOAT)
                    i = i + 6;
                for (int tmp_i = 0; tmp_i < event.offset; tmp_i++)
#ifndef RING_USE_ARR
                    tmp_ringP = tmp_ringP->next;
                if (tmp_ringP->next == NULL)
#else
                    tmp_ringP = (tmp_ringP + 1) % RING_BUFF_LENGTH;
                if (tmp_ringP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
                    return event;
            }
        }
#ifndef RING_USE_ARR
        else if (tmp_ringP->data != tmp_cmdP->cmd_String[i])
#else
        else if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[i] != uart_ring_param_P->ringBuffArr[tmp_ringP])
#endif
        {
            event.state = RING_ERR;
            return event;
        }
    }
    // if(isDelRingHeadP == 0)
    // else
    event.state = RING_WAIT;
    return event;
}

static callBackFun_EventTypedef intFlagJudge(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    ringNodeTypedef *ringNodeP, cmdNodeTypedef *cmdNodeP,
#else
    int ringNodeP, int cmdIndex,
#endif
    int index)
{
    callBackFun_EventTypedef event;
#ifndef RING_USE_ARR
    if (cmdNodeP->cmd_String[index] != '{')
    {
        event.state = RING_WAIT;
        return event;
    }
    if (cmdNodeP->cmd_String[index + 1] == INT_FLAG[1] &&
        cmdNodeP->cmd_String[index + 2] == INT_FLAG[2] &&
        cmdNodeP->cmd_String[index + 3] == INT_FLAG[3] &&
        cmdNodeP->cmd_String[index + 4] == INT_FLAG[4])
#else
    if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index] != '{')
    {
        event.state = RING_WAIT;
        return event;
    }
    if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 1] == INT_FLAG[1] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 2] == INT_FLAG[2] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 3] == INT_FLAG[3] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 4] == INT_FLAG[4])
#endif
    {
        int cnt = 0;
#ifndef RING_USE_ARR
        if (ringNodeP->next == NULL)
#else
        if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
        {
            event.state = RING_WAITFORFLAG;
            return event;
        }
#ifndef RING_USE_ARR
        ringNodeTypedef *tmp_ringP = ringNodeP->next;
#else
        ringNodeP = (ringNodeP + 1) % RING_BUFF_LENGTH;
#endif
        int arrBuff[CMD_STRING_LENGTH];
        for (int i = 0; i < CMD_STRING_LENGTH; i++)
        {
#ifndef RING_USE_ARR
            uint8_ring data = tmp_ringP->data;
#else
            uint8_ring data = uart_ring_param_P->ringBuffArr[ringNodeP];
#endif
            if ('0' <= data && data <= '9')
            {
                arrBuff[i] = data - '0';
#ifndef RING_USE_ARR
                if (tmp_ringP->next == NULL)
#else
                if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
                {
                    event.state = RING_WAITFORFLAG;
                    return event;
                }
#ifndef RING_USE_ARR
                tmp_ringP = tmp_ringP->next;
#else
                ringNodeP++;
#endif
            }
            else if (data == '}')
            {
                cnt = i;
                event.state = RING_INT;
                event.offset = cnt;
                break;
            }
            else
            {
#ifndef RING_USE_ARR
                if (tmp_ringP->next == NULL)
#else
                if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
                {
                    event.state = RING_WAITFORFLAG;
                    return event;
                }
                event.state = RING_ERR; // cmp next cmd
                return event;
            }
        }
        event.intData = 0;
        int tmp = 1;
        for (int i = 0; i < cnt; i++)
        {
            event.intData += arrBuff[cnt - i - 1] * tmp;
            tmp *= 10;
        }
        return event;
    }
    else
#ifndef RING_USE_ARR
        return floatFlagJudge(uart_ring_param_P, ringNodeP, cmdNodeP, index);
#else
        return floatFlagJudge(uart_ring_param_P, ringNodeP, cmdIndex, index);
#endif
}

static callBackFun_EventTypedef floatFlagJudge(
    uartRingParameterTypedef *uart_ring_param_P,
#ifndef RING_USE_ARR
    ringNodeTypedef *ringNodeP, cmdNodeTypedef *cmdNodeP,
#else
    int ringNodeP, int cmdIndex,
#endif
    int index)
{
    callBackFun_EventTypedef event;
#ifndef RING_USE_ARR
    if (cmdNodeP->cmd_String[index] != '{')
    {
        event.state = RING_WAIT;
        return event;
    }
    if (cmdNodeP->cmd_String[index + 1] == FLOAT_FLAG[1] &&
        cmdNodeP->cmd_String[index + 2] == FLOAT_FLAG[2] &&
        cmdNodeP->cmd_String[index + 3] == FLOAT_FLAG[3] &&
        cmdNodeP->cmd_String[index + 4] == FLOAT_FLAG[4] &&
        cmdNodeP->cmd_String[index + 5] == FLOAT_FLAG[5] &&
        cmdNodeP->cmd_String[index + 6] == FLOAT_FLAG[6])
#else
    if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index] != '{')
    {
        event.state = RING_WAIT;
        return event;
    }
    if (uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 1] == FLOAT_FLAG[1] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 2] == FLOAT_FLAG[2] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 3] == FLOAT_FLAG[3] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 4] == FLOAT_FLAG[4] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 5] == FLOAT_FLAG[5] &&
        uart_ring_param_P->cmdBuffArr[cmdIndex].cmd_String[index + 6] == FLOAT_FLAG[6])
#endif
    {
        int cnt = 0;
#ifndef RING_USE_ARR
        if (ringNodeP->next == NULL)
#else
        if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
        {
            event.state = RING_WAITFORFLAG;
            return event;
        }
#ifndef RING_USE_ARR
        ringNodeTypedef *tmp_ringP = ringNodeP->next;
#else
        ringNodeP = (ringNodeP + 1) % RING_BUFF_LENGTH;
#endif
        char strBuff[CMD_STRING_LENGTH];
        for (int i = 0; i < CMD_STRING_LENGTH; i++)
        {
#ifndef RING_USE_ARR
            uint8_ring data = tmp_ringP->data;
#else
            uint8_ring data = uart_ring_param_P->ringBuffArr[ringNodeP];
#endif
            if ('0' <= data && data <= '9' || data == '.')
            {
                strBuff[i] = data;
#ifndef RING_USE_ARR
                if (tmp_ringP->next == NULL)
#else
                if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
                {
                    event.state = RING_WAITFORFLAG;
                    return event;
                }
#ifndef RING_USE_ARR
                tmp_ringP = tmp_ringP->next;
#else
                ringNodeP++;
#endif
            }
            else if (data == '}')
            {
                cnt = i;
                event.state = RING_OK;
                event.offset = cnt;
                strBuff[i] = 0;
                break;
            }
            else
            {
#ifndef RING_USE_ARR
                if (tmp_ringP->next == NULL)
#else
                if (ringNodeP == (uart_ring_param_P->ringBuffTail + 1) % RING_BUFF_LENGTH)
#endif
                {
                    event.state = RING_WAITFORFLAG;
                    return event;
                }
                event.state = RING_ERR; // cmp next cmd
                return event;
            }
        }
        event.floatData = (float)atof(strBuff);
        if (event.floatData == (float)0.0)
        {
            event.state = RING_ERR;
        }
        else
            event.state = RING_FLOAT;
        return event;
    }
    event.state = RING_ERR; // cmp next cmd
    return event;
}
