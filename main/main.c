#include <stdio.h>
#include "Inf/Inf_BDR6120.h"
#include "Inf/Inf_WTN6170.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Dri/Dri_NVS.h"

void app_main(void)
{
    // Inf_BDR6120_Init();
    // Inf_BDR6120_Open();

    // Inf_WTN6170_Init();
    // Inf_WTN6170_SendCmd(0xF3);
    // Inf_WTN6170_SendCmd(65);

    // Inf_WTN6170_SendCmd(0xF3);
    // Inf_WTN6170_SendCmd(64);

    Dri_NVS_Init();

    esp_err_t err = Dri_NVS_IsKeyExist("admin");
    if(err == ESP_OK)
    {
        printf("is exist \r\n");
    }
    else
    {
        printf("no exist \r\n");
    }

    uint8_t value[3] = { '1','2','3' };
    Dri_NVS_WriteStr("aaa",value);
    uint8_t recedata[20] = {0};
    uint8_t len = 20;
    Dri_NVS_ReadStr("aaa",recedata,&len);
    printf("str = %s \r\n",recedata);
    
    while(1){
        vTaskDelay(100);
    }
}
