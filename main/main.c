#include <stdio.h>
#include "Inf/Inf_BDR6120.h"
#include "Inf/Inf_WTN6170.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Dri/Dri_NVS.h"
#include "App/App_IO.h"

void key_scan_task(void * args);
TaskHandle_t keyScanHandler;

void app_main(void)
{
    //初始化所有
    App_IO_Init();

    //创建一个读按键的任务
    xTaskCreate(key_scan_task,
                "key_scan_task",
                2048,
                NULL,
                5,
                &keyScanHandler);  //优先级最高24
}

void key_scan_task(void *)
{
    uint8_t pwd[100] = {0};
    
    //循环等待键入
    while(1)
    {
        Com_Status status = App_IO_ReadStr(pwd);

        switch(status)
        {
            case Com_OK://以*结束
                App_IO_Handle(pwd);
                break;

            case Com_ERROR://以M结束
                sayIllegalOperation();
                break;

            case Com_TIMEOUT:
                sayInputUserPasswordAgain();
                break;
            
            default:
                break;
        }

        memset(pwd,0,sizeof(pwd)); //清零

        vTaskDelay(10); //10ms
    }
}

// void app_main(void)
// {
//     // Inf_BDR6120_Init();
//     // Inf_BDR6120_Open();

//     // Inf_WTN6170_Init();
//     // Inf_WTN6170_SendCmd(0xF3);
//     // Inf_WTN6170_SendCmd(65);

//     // Inf_WTN6170_SendCmd(0xF3);
//     // Inf_WTN6170_SendCmd(64);

//     Dri_NVS_Init();

//     esp_err_t err = Dri_NVS_IsKeyExist("admin");
//     if(err == ESP_OK)
//     {
//         printf("is exist \r\n");
//     }
//     else
//     {
//         printf("no exist \r\n");
//     }

//     uint8_t value[3] = { '1','2','3' };
//     Dri_NVS_WriteStr("aaa",value);
//     uint8_t recedata[20] = {0};
//     uint8_t len = 20;
//     Dri_NVS_ReadStr("aaa",recedata,&len);
//     printf("str = %s \r\n",recedata);
    
//     while(1){
//         vTaskDelay(100);
//     }
// }
