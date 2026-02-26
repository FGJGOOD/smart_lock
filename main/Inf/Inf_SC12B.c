#include "Inf_SC12B.h"

void Inf_SC12B_Init(void)
{
    //准备配置信息
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SC12B_SDA;
    conf.scl_io_num = I2C_SC12B_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    i2c_param_config(I2C_NUM_0, &conf);

    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);

    Inf_SC12B_WriteReg(0x00, 0x04); //灵敏度
    Inf_SC12B_WriteReg(0x01, 0x04);

}

void Inf_SC12B_WriteReg(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};

    i2c_master_write_to_device( I2C_NUM_0, 
                                SC12B_I2C_ADDR, 
                                write_buf, 
                                sizeof(write_buf), 
                                2000);

}

uint8_t Inf_SC12B_ReadReg(uint8_t reg_addr)
{
    uint8_t data = 0;
    esp_err_t ret = i2c_master_write_read_device(I2C_NUM_0,
        SC12B_I2C_ADDR,
        &reg_addr,
        1,
        &data,
        1,
        2000);
    return data;
}

Touch_Key Inf_SC12B_KeyClick(void)
{
    uint16_t key = (Inf_SC12B_ReadReg(0x08) << 8) | (Inf_SC12B_ReadReg(0x09));
    Touch_Key key_click = KEY_NO; //默认

    switch(key){
        case 0x8000:
            key_click = KEY_0;
            break;
        case 0x4000:
            key_click = KEY_1;
            break;
        case 0x2000:
            key_click = KEY_2;
            break;
        case 0x1000:
            key_click = KEY_3;
            break;
        case 0x0800:
            key_click = KEY_4;
            break;
        case 0x0400:
            key_click = KEY_5;
            break;
        case 0x0200:
            key_click = KEY_6;
            break;
        case 0x0100:
            key_click = KEY_7;
            break;
        case 0x0080:
            key_click = KEY_8;
            break;
        case 0x0040:
            key_click = KEY_9;
            break;
        case 0x0020:
            key_click = KEY_SHARP;
            break;
        case 0x0010:
            key_click = KEY_M;
            break;


        default:
            break;
        
    }


    return key_click;
}