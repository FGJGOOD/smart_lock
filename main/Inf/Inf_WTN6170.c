#include "Inf_WTN6170.h"

void Inf_WTN6170_Init(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pin_bit_mask = (1ULL << WTN6170_DATA_PIN);

    gpio_config(&io_conf);

    WTN6170_DATA_H;
}

void Inf_WTN6170_SendCmd(uint8_t cmd)
{
    WTN6170_DATA_L;
    vTaskDelay(10); 

    for(uint8_t i = 0; i < 8;i++)//send 8 bits
    {
        if(cmd & (1 << i))
        {
            WTN6170_DATA_H;
            usleep(600);
            WTN6170_DATA_L;
            usleep(200);
        }
        else
        {
            WTN6170_DATA_H;
            usleep(200);
            WTN6170_DATA_L;
            usleep(600);
        }
    }

    WTN6170_DATA_H;
    vTaskDelay(5);
}
