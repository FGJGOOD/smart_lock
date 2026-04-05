#include  "App_IO.h"


uint8_t first_buffer[100] = {0}; //两个数组用作输入和比对
uint8_t second_buffer[100] = {0};

static void App_IO_ClearBuffer(void)
{
    memset(first_buffer,0,sizeof(first_buffer));
    memset(second_buffer,0,sizeof(second_buffer));
}

//******************具体操作封装**************************** */
static void App_IO_AddAdmin(void)
{
    if(Dri_NVS_IsKeyExist((uint8_t*)Admin_PWD) == ESP_OK) //如果管理员用户已存在，就不添加
    {
        sayAdminFull();
    }
    else
    {
        sayWithoutInt();
        sayAddAdmin();
        sayWithoutInt();
        sayInputAdminPassword();
        //输入管理员密码
        Com_Status comstatus = App_IO_ReadStr(first_buffer);

        switch(comstatus)
        {
            case Com_OK:
                //再次输入管理员密码
                sayInputAdminPasswordAgain();
                Com_Status comstatus = App_IO_ReadStr(second_buffer); //读按键输入
                switch(comstatus)
                {
                    case Com_OK:
                        //比较两次输入是否一致
                        if(strcmp((char*)first_buffer,(char*)second_buffer) == 0)
                        {
                            //比较一致就写入FLASH
                            esp_err_t err = Dri_NVS_WriteStr((uint8_t*)Admin_PWD,first_buffer);
                            if(err == ESP_OK)//这里判断是否成功写入FLASH
                            {
                                sayAddSucc();
                            }
                            else
                            {
                                sayAddFail();
                            }
                        }
                        else
                        {
                            sayAddFail();
                        }

                        break;
                    case Com_ERROR:
                        sayIllegalOperation();
                        break;
                    default:
                        break;
                }
                break;

            case Com_ERROR:
                sayIllegalOperation();
                break;

            default:
                break;
        }
    
        App_IO_ClearBuffer(); //每一次操作结束都清空两个buffer
    }
}
//验证管理员
static Com_Status App_IO_CheckAdmin(void)
{
    sayWithoutInt();
    sayInputAdminPassword();

    Com_Status comstatus = App_IO_ReadStr(first_buffer); //读取按键输入  通过循环等待输入
    switch(comstatus)
    {
        case Com_OK:
            uint8_t len = 0;
            Dri_NVS_ReadStr((uint8_t*)Admin_PWD,second_buffer,&len);  //读取放到second_buffer中

            if(strcmp((char*)first_buffer,(char*)second_buffer) == 0)
            {
                App_IO_ClearBuffer();     //返回前先清空
                return Com_OK;
            }
            // else
            // {
            //     return Con_ERROR;
            // }

            break;
        case Com_ERROR:
            sayIllegalOperation();
            break;
        default:
            break;
    }

    App_IO_ClearBuffer(); //这个需要放到return Com_OK 前不？

    return Com_ERROR;
}

static void App_IO_DelAdmin( void )
{
    if(Dri_NVS_IsKeyExist((uint8_t*)Admin_PWD) != ESP_OK) //管理员账户不存在，无法删除
    {
        sayIllegalOperation();
    }
    else
    {
        sayWithoutInt();
        sayDelAdmin();

        Com_Status comstatus = App_IO_CheckAdmin();//输入密码 ，验证管理员
        if(comstatus == Com_OK)
        {
            esp_err_t err = Dri_NVS_DeleKey((uint8_t*)Admin_PWD);
            if(err == ESP_OK)
            {
                sayDelSucc();
            }
            else
            {
                sayDelFail();
            }
        }
        else
        {
            sayDelFail();
        }
    }
}
static void App_IO_AddUser(void)
{
    if(Dri_NVS_IsKeyExist((uint8_t*)Admin_PWD) != ESP_OK)
    {
        sayIllegalOperation(); //要现有管理员才能有添加用户的权限
    }
    else
    {
        sayWithoutInt();
        sayAddUser();

        Com_Status comstatus = App_IO_CheckAdmin();
        if(comstatus == Com_OK)
        {
            comstatus = App_IO_ReadStr(first_buffer);
            switch(comstatus)
            {
                case Com_OK:
                    //再次输入
                    sayWithoutInt();
                    sayInputUserPasswordAgain();
                    comstatus = App_IO_ReadStr(second_buffer);
                    switch(comstatus)
                    {
                        case Com_OK:
                            //比较
                            if(strcmp((char*)first_buffer,(char*)second_buffer) == 0)
                            {
                                esp_err_t err = Dri_NVS_WriteStr(first_buffer,(uint8_t*)"0");
                                if(err == ESP_OK)
                                {
                                    sayAddSucc();
                                }
                                else
                                {
                                    sayAddFail();
                                }
                            }
                            else
                            {
                                sayAddFail();
                            }
                            break;
                        case Com_ERROR:
                            sayIllegalOperation();
                            break;
                        default:
                            break;
                    }

                    break;
                case Com_ERROR:
                    sayIllegalOperation();
                    break;
                default:
                    break;
            }
        
        }
        else
        {
            sayAddFail();
        }

        App_IO_ClearBuffer();
    }

}
static void App_IO_DelUser(void)
{
    if(Dri_NVS_IsKeyExist((uint8_t*)Admin_PWD) != ESP_OK)
    {
        sayIllegalOperation(); //要现有管理员才能有删除用户的权限
    }
    else
    {
        sayWithoutInt();
        sayDelUser();

        Com_Status comstatus = App_IO_CheckAdmin(); //先验证管理员
        if(comstatus == Com_OK)
        {
            sayInputUserPassword();
            comstatus = App_IO_ReadStr(first_buffer);
            switch(comstatus)
            {
                case Com_OK:
                    if(Dri_NVS_IsKeyExist(first_buffer) == ESP_OK) //确实有该用户存在
                    {
                        esp_err_t err = Dri_NVS_DeleKey(first_buffer);
                        if(err == ESP_OK)
                        {
                            sayDelSucc();
                        }
                        else
                        {
                            sayDelFail();
                        }
                    }
                    else //没有该用户
                    {
                        sayDelFail();
                    }
                    break;
                case Com_ERROR:
                    sayIllegalOperation();
                    break;
                default:
                    break;
            }
        }
        else
        {
            sayDelFail();
        }
    }

    App_IO_ClearBuffer();
}

static void App_IO_CheckUser(uint8_t pwd[])
{
    if(Dri_NVS_IsKeyExist(pwd) == ESP_OK)
    {
        sayVerifySucc();
        Inf_BDR6120_Open(); // 开锁
        sayDoorOpen();
    }
    else
    {
        sayWithoutInt();
        sayVerifyFail();
        sayWithoutInt();
        sayRetry();
    }
}

void App_IO_Init(void)
{
    Inf_BDR6120_Init(); //电机

    Inf_WTN6170_Init(); //语音

    Inf_SC12B_Init(); //按键

    Dri_NVS_Init(); //flash
}

Com_Status App_IO_ReadStr(uint8_t pwd[])
{
    uint8_t len = 0;
    uint8_t noKeyTime = 0;
    while(1)
    {
        Touch_Key touchkey = Inf_SC12B_GetKeyClick();
        
        if(touchkey == KEY_NO) //没有按键按下
        {
            noKeyTime++;
            if(noKeyTime >= 100) //超多5s没有按键按下
            {
                return Com_TIMEOUT;
            }
        }
        else
        {
            sayWaterDrop(); //水滴声

            noKeyTime = 0; //有按键按下，清空计时

            if(touchkey == KEY_M)
            {
                return Com_ERROR;
            }
            else if(touchkey == KEY_SHARP)
            {
                return Com_OK;
            }
            else
            {
                pwd[len++] = touchkey + 48;//数字转字符
            }
        }

        vTaskDelay(50);
    }
}

//处理按键组合
// 00# 注册管理员 01#删除管理员 10#注册用户 11#删除用户 99#删除所有 #是不存入数组pwd中的
void App_IO_Handle(uint8_t pwd[])
{
    uint8_t len = strlen((char*)pwd);
    
    if(len < 2)  //只键入一个或没有键入
    {
        sayIllegalOperation();
    }
    else if (len == 2)
    {
        if(pwd[0] == '0' && pwd[1] == '0')
        {
            App_IO_AddAdmin();
        }
        else if(pwd[0] == '0' && pwd[1] == '1')
        {
            App_IO_DelAdmin();
        }
        else if(pwd[0] == '1' && pwd[1] == '0')
        {
            App_IO_AddUser();
        }
        else if(pwd[0] == '1' && pwd[1] == '1')
        {
            App_IO_DelUser();
        }
        else if(pwd[0] == '9' && pwd[1] == '9')
        {
            sayDelAll();//语音播报删除所有
            Dri_NVS_DeleAll();
        }
        else //指令不存在
        {
            sayIllegalOperation();
        }
    }
    else
    {
        App_IO_CheckUser(pwd); //验证密码开锁
    }
    
}
