#include <stdio.h>
#include "Inf/Inf_BDR6120.h"
#include "Inf/Inf_WTN6170.h"


void app_main(void)
{
    // Inf_BDR6120_Init();
    // Inf_BDR6120_Open();

    Inf_WTN6170_Init();
    Inf_WTN6170_SendCmd(0xF3);
    Inf_WTN6170_SendCmd(65);

    Inf_WTN6170_SendCmd(0xF3);
    Inf_WTN6170_SendCmd(64);
    
    while(1){
        vTaskDelay(100);
    }
}
