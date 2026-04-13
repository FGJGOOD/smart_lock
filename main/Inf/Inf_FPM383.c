#include "Inf_FPM383.h"

static const int  RX_BUF_SIZE = 1024;
static uint8_t receData[100] = {1};//默认为0的话可能会导致意外的麻烦 芯片操作成功会返回00

uint8_t hasFinger = 0;
static void Inf_FPM383_Intr_Handler(void* arg) //中断服务函数？
{

    hasFinger = 1;
}

static Com_Status Inf_FPM383_SendData(uint8_t data[],uint8_t len)
{
    uint8_t sendlen = uart_werite_bytes(UART_NUM_1, data, len); //发送数据
    if(sendlen == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}
static Com_Status Inf_FPM383_ReceData(uint8_t len,uint16_t timeout)//要接受的数据字节数，超时时间
{
    memset(receData,1,sizeof(receData));
    uint8_t recelen = uart_read_bytes(UART_NUM_1,receData,len,timeout)
    if(recelen == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}
static void Inf_FPM383_AddCheckSum(uint8_t cmd[],uint8_t len)//指令集，指令集长度
{
    //校验和为2字节
    uint16_t sum = 0;
    
    //计算校验和
    for(uint8_t i = 6;i < len - 2;i++)
    {
        sum += cmd[i];
    }

    //将校验和添加至指令集末尾
    cmd[len - 2] = (sum >> 8);//高8位
    cmd[len - 1] = sum;       //低8位
}

void Inf_FPM383_Init(void)
{
    //串口参数配置
    const uart_config_t uart_config = {
        .baud_rate = 57600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    //驱动安装
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    //使用配置的参数
    uart_param_config(UART_NUM_1, &uart_config);
    //引脚设置
    uart_set_pin(UART_NUM_1, INF_FPM383_TX_PIN, INF_FPM383_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //中断配置
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1 << INF_FPM383_INTR_PIN),
        .pull_up_en = 0,
        .pull_down_en = 1,
    };
    gpio_config(&io_config);
    
    //安装中断服务
    gpio_install_isr_service(0);

    //注册回调函数
    gpio_isr_handler_add(INF_FPM383_INTR_PIN, Inf_FPM383_Intr_Handler, (void*)INF_FPM383_INTR_PIN);

    //关闭中断
    gpio_intr_disable(INF_FPM383_INTR_PIN);

    /* 进入休眠模式 */
    Inf_FPM383_Sleep();
}

void Inf_FPM383_Sleep(void)
{
    uint8_t cmd[12] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x03,           //包长度
        0x33,                //指令码
        0x00, 0x37           //校验码
    };

    do{
        MY_LOGE("等待进入休眠中。。。");
        Inf_FPM383_SendData(cmd,12); //发送数据
        Inf_FPM383_ReceData(12,2000);//接收应答，给2s时延
    }while(receData[9] != 0x00);     //看接收应答的第9位是否为00，否则循环发送指令，确保芯片进入休眠模式

    MY_LOGE("休眠成功！");  //发送任何指令可唤醒
    //进入休眠，开启中断
    gpio_intr_enable(INF_FPM383_INTR_PIN);

}

void Inf_FPM383_ReadId(void)
{
    uint8_t cmd[13] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x04,           //包长度
        0x34,                //指令码
        0x00,                //参数
        0x00, 0x39           //校验码
    };

    Inf_FPM383_SendData(cmd,13);

    Inf_FPM383_ReceData(44,3000);  //应答数据长度为44字节

    if(receData[9] == 0x00) //应答成功
    {
        MY_LOGE("唯一ID为 %.32s",&receData[10]);
    }
    else
    {
        MY_LOGE("获取ID失败");
    }

}

//获取最小可用的ID
uint16_t Inf_FPM383_GetMinId(void)
{
    uint8_t cmd[13] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x04,           //包长度
        0x1F,                //指令码
        0x00,                //页码          每个页码可放0~255个指纹模板
        '\0', '\0'           //校验和   先使用 '\0' 占位        不同页码对应不同检验和，可以单独设计函数来计算校验和
    };

    Inf_FPM383_AddCheckSum(cmd,13); 

    Inf_FPM383_SendData(cmd,13);

    Inf_FPM383_ReceData(44,3000); //应答包的第10位开始的32个字节就是索引表

    for(uint8_t i = 0;i < 32;i++)
    {
        uint8_t byte = receData[10 + i];

        for(uint8_t j = 0;j < 8;j++)
        {
            if(byte & 0x01)
            {
                byte >>= 1;
            }
            else
            {
                return i*8 + j;
            }
        }
    }
    //如果全部索引都为1，返回一个默认索引
    return 0;
}

void Inf_FPM383_CancelAutoAction(void)
{
    uint8_t cmd[12] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x03,           //包长度
        0x30,                //指令码        
        '\0', '\0'           //校验和   先使用 '\0' 占位        不同页码对应不同检验和，可以单独设计函数来计算校验和
    };

    Inf_FPM383_AddCheckSum(cmd,12);

    do{
    Inf_FPM383_SendData(cmd,12);
    Inf_FPM383_ReceData(12,2000);
    }while(receData[9] != 0x00); //循环发送指令确保成功 

    MY_LOGE("取消自动操作成功...");
}

//一站式录入指纹
Com_Status Inf_FPM383_AddFingerPrint(int16_t id)
{
    uint8_t cmd[17] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x08,           //包长度
        0x31,                //指令码
        '\0','\0',           //ID号
        0x01,                //采集次数
        0x00,0x32,           //参数                      是否开启LED，是否重复录入，是否覆盖ID ...        
        '\0', '\0'           //校验和   先使用 '\0' 占位        不同页码对应不同检验和，可以单独设计函数来计算校验和
    };

    cmd[9] = (id >> 8);
    cmd[10] = id;
    Inf_FPM383_AddCheckSum(cmd,17);

    //取消自动操作
    Inf_FPM383_CancelAutoAction();
    Inf_FPM383_CancelAutoAction();
    Inf_FPM383_CancelAutoAction();
    Inf_FPM383_CancelAutoAction();

    //发送指令集
    Inf_FPM383_SendData(cmd,17);

    //接收应答包，如果有一个返回失败，就时失败
    while(1)
    {
        Inf_FPM383_ReceData(14,2000);

        if(receData[9] != 0x00)
        {
            return Com_ERROR;
        }
        else if (receData[10] == 0x06) //返回成功且是最后一个包，就代表成功录入指纹
        {
            return Com_OK;
        }    
    }
    
}

//验证指纹
Com_Status Inf_FPM383_CheckFingerPrint(void)
{
    uint8_t cmd[17] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x08,           //包长度
        0x32,                //指令码
        0x03,                //分数等级
        0xFF,0xFF,               //ID号                  与所有指纹进行比对
        0x00,0x06,           //参数                      是否开启LED，是否重复录入，是否覆盖ID ...        
        '\0', '\0'           //校验和   先使用 '\0' 占位        
    };

    Inf_FPM383_AddCheckSum(cmd,17);

    Inf_FPM383_SendData(cmd,17);

    Inf_FPM383_ReceData(17,2000);

    if(receData[9] == 0x00)//成功
    {
        MY_LOGE("Check ID =  %d",receData[12]); //输出ID
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

int16_t Inf_FPM383_GetFingerPrintId(void)
{
    uint8_t cmd[17] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x08,           //包长度
        0x32,                //指令码
        0x03,                //分数等级
        0xFF,0xFF,               //ID号                  与所有指纹进行比对
        0x00,0x06,           //参数                      是否开启LED，是否重复录入，是否覆盖ID ...        
        '\0', '\0'           //校验和   先使用 '\0' 占位        
    };

    Inf_FPM383_AddCheckSum(cmd,17);

    Inf_FPM383_SendData(cmd,17);

    Inf_FPM383_ReceData(17,2000);

    if(receData[9] == 0x00)//成功
    {
        return receData[12];
    }
    else
    {
        return -1; //有负数所以用int型返回
    }    
}

Com_Status Inf_FPM383_DeleFingerPrint(int16_t id)
{
    uint8_t cmd[16] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x07,           //包长度
        0x0C ,                //指令码
        '\0','\0',                //PageID
        0x00,0x01,               //删除个数
        '\0', '\0'           //校验和   先使用 '\0' 占位        
    };

    cmd[10] = (id >> 8);                         //我自己添加
    cmd[11] = id;

    Inf_FPM383_AddCheckSum(cmd,16);                 //这里还没添加要删的id？？？？？？

    Inf_FPM383_SendData(cmd,16);

    Inf_FPM383_ReceData(12,2000);

    if(receData[9] == 0x00)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

void Inf_FPM383_DeleAllFingerPrint(void)
{
    uint8_t cmd[12] = {
        0xEF,0x01,           //包头
        0xFF,0xFF,0xFF,0xFF, //设备地址
        0x01,                //包标识
        0x00,0x03,           //包长度
        0x0D ,                //指令码
        0x00, 0x01           //校验和          
    };


    do{
        Inf_FPM383_SendData(cmd,12);

        Inf_FPM383_ReceData(12,2000);
    }while(receData[9] != 0x00);

    MY_LOGE("清空   成功...");

}