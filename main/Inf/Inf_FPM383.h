#ifndef __INF_FPM383_H__
#define __INF_FPM383_H__


#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "../Common/Com_Config.h"
#include "../Common/Com_Debug.h"

#define INF_FPM383_TX_PIN GPIO_NUM_21
#define INF_FPM383_RX_PIN GPIO_NUM_20
#define INF_FPM383_INTR_PIN GPIO_NUM_10

void Inf_FPM383_Init(void);

void Inf_FPM383_Sleep(void); //使用串口发送相关指令并检验接收数据来确保进入休眠模式

void Inf_FPM383_ReadId(void);

uint16_t Inf_FPM383_GetMinId(void);

void Inf_FPM383_CancelAutoAction(void);

Com_Status Inf_FPM383_AddFingerPrint(uint16_t id);

Com_Status Inf_FPM383_CheckFingerPrint(void);

int16_t Inf_FPM383_GetFingerPrintId(void);

Com_Status Inf_FPM383_DeleFingerPrint(uint16_t id);

void Inf_FPM383_DeleAllFingerPrint(void);

#endif