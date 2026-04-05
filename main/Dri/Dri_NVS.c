#include "Dri_NVS.h"

static nvs_handle_t my_handle;

void Dri_NVS_Init(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    //打开一块空间
    nvs_open("storage", NVS_READWRITE, &my_handle);
}

esp_err_t Dri_NVS_WriteStr(uint8_t key[], uint8_t value[])
{
    return nvs_set_str(my_handle, (char*)key,(char*)value);
}

esp_err_t Dri_NVS_ReadStr(uint8_t key[], uint8_t value[],uint8_t *len)
{
    return nvs_get_str(my_handle,(char*)key,(char*)value,(size_t*)len);
}

esp_err_t Dri_NVS_DeleKey(uint8_t key[])
{
    return nvs_erase_key(my_handle,(char*)key);
}

esp_err_t Dri_NVS_DeleAll()
{
    return nvs_erase_all(my_handle);
}

esp_err_t Dri_NVS_IsKeyExist(uint8_t key[])
{
    return nvs_find_key(my_handle,(char*)key,NULL);
}
