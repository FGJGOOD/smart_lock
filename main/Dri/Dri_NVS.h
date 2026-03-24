#ifndef __DRI_NVS_H__
#define __DRI_NVS_H__

#include <stdio.h>
#include <inttypes.h>

#include "nvs_flash.h"
#include "nvs.h"

void Dri_NVS_Init(void);

esp_err_t Dri_NVS_WriteStr(uint8_t key[], uint8_t value[]);

esp_err_t Dri_NVS_ReadStr(uint8_t key[], uint8_t value[],size_t * len);

esp_err_t Dri_NVS_DeleKey(uint8_t key[]);

esp_err_t Dri_NVS_DeleAll();

esp_err_t Dri_NVS_IsKeyExist(uint8_t key[]);
#endif
