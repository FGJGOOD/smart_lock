#ifndef __INF_BDR6120_H__
#define __INF_BDR6120_H__ 

#include "driver/gpio.h"                                  /* 为什么不用父目录 */
#include "../Common/Com_Debug.h"
#include "esp_task.h"

#define INF_BDR_INA GPIO_NUM_4
#define INF_BDR_INB GPIO_NUM_5

void Inf_BDR6120_Init(void);

void Inf_BDR6120_Forward(void);

void Inf_BDR6120_Back(void);

void Inf_BDR6120_Open(void);
void Inf_BDR6120_Break(void);





#endif
