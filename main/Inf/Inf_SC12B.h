#ifndef __INF_SC12B_H
#define __INF_SC12B_H

#include "../Common/Com_Debug.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

#define I2C_SC12B_SDA GPIO_NUM_2
#define I2C_SC12B_SCL GPIO_NUM_1
#define I2C_SC12B_INTR GPIO_NUM_0

#define I2C_MASTER_FREQ_HZ 100000

#define SC12B_I2C_ADDR (0x42)  //0x40 或者 0x42

typedef enum{
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SHARP,
    KEY_M,
    KEY_NO
}Touch_Key;

void Inf_SC12B_Init(void);

void Inf_SC12B_WriteReg(uint8_t reg_addr, uint8_t data);

uint8_t Inf_SC12B_ReadReg(uint8_t reg_addr);

Touch_Key Inf_SC12B_KeyClick(void);

#endif
