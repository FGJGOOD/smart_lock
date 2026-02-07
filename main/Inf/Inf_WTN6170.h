#ifndef __INF_WTN6170_H
#define __INF_WTN6170_H


#include "driver/gpio.h"
#include "sys/unistd.h"
#include "esp_task.h"

#define WTN6170_DATA_PIN GPIO_NUM_9
#define WTN6170_DATA_L (gpio_set_level(WTN6170_DATA_PIN,0))
#define WTN6170_DATA_H (gpio_set_level(WTN6170_DATA_PIN,1))

void Inf_WTN6170_Init(void);

void Inf_WTN6170_SenCmd(uint8_t cmd);


#endif
