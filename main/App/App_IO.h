#ifndef __APP_IO_H__
#define __APP_IO_H__

#include "../Inf/Inf_BDR6120.h"
#include "../Inf/Inf_WTN6170.h"
#include "../Inf/Inf_SC12B.h"
#include "../Inf/Inf_FPM383.h"
#include "../Dri/Dri_NVS.h"
#include "../Common/Com_Config.h"

#define Admin_PWD "admin"

void App_IO_Init(void);

Com_Status App_IO_ReadStr(uint8_t pwd[]);

void App_IO_Handle(uint8_t pwd[]); //处理不同的输入

void App_IO_Finger(void);

#endif
