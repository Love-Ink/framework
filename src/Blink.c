#include "Blink.h"
#include "string.h"
#include "stdlib.h"

static Blink_Info *Blink_head = NULL; 

#define true 1
#define false 0

/**
 * @brief 创建Blink设备
 * 
 * @param Name Blink设备名
 * @param ioctrl 控制函数
 * @param Get_tick 获取Tick函数,之后每一个单位与你Tick变化时间绑定
 */
void BlinkDevice_Creat(const char *Name, void(*ioctrl)(uint8_t state), uint32_t (*Get_tick)(void)) {  
    //没有 配置设备名 退出
    if(strlen((const char *)Name) == 0) return ; 

    //没有配置 设备控制函数 退出
    if(ioctrl == NULL) return ; 

    //没有配置 Tick来源函数 退出
    if(Get_tick == NULL) return ; 

    //开辟内存
    Blink_Info *Creat_Blink = malloc(sizeof(Blink_Info));
    //初始化结构体
    memset(Creat_Blink, 0, sizeof(Blink_Info));
    
    memcpy(Creat_Blink->Name, Name, strlen((const char *)Name));
    Creat_Blink->Blink_Cfg.ioctrl = ioctrl;
    Creat_Blink->Blink_Cfg.Get_tick = Get_tick;
    Creat_Blink->Blink_State.OffTime = 10;
    Creat_Blink->Blink_Cfg.ioctrl(false); //初始化

    if(Blink_head == NULL) { //如果首次创建
        Blink_head = Creat_Blink;
    } else { //链表尾连接新链表 
        Blink_Info *Temp = Blink_head;
        while(Temp->Blink_next != NULL) Temp = Temp->Blink_next;
        Temp->Blink_next = Creat_Blink;
    }
}

/**
 * @brief Blink设备扫描
 * 
 */
void BlinkDevice_Scan(uint16_t ms) {
    //如果未创建过设备，退出
    if(Blink_head == NULL) return ;

    
    Blink_Info *Blink_Device = Blink_head;
    while(Blink_Device != NULL) {
        if (Blink_Device->Blink_State.OnTime == 0) {//开启时间为0

        } else if (Blink_Device->Blink_Cfg.Get_tick() - Blink_Device->Blink_State.tick < Blink_Device->Blink_State.OnTime) { //当前是否为开启
            if (!Blink_Device->Blink_State.State) { //第一次进入
                Blink_Device->Blink_State.State = true;
                Blink_Device->Blink_Cfg.ioctrl(true);
            }
        } else if (Blink_Device->Blink_Cfg.Get_tick() - Blink_Device->Blink_State.tick < Blink_Device->Blink_State.OffTime) { //当前是否为关闭
            if (Blink_Device->Blink_State.State) { //第一次进入
                Blink_Device->Blink_State.State = false;
                Blink_Device->Blink_Cfg.ioctrl(false);
            }
        } else { //一个周期结束
            Blink_Device->Blink_State.tick = Blink_Device->Blink_Cfg.Get_tick(); //获取当前tick
            if (Blink_Device->Blink_State.repeats) { //是否有配置限制周期次数
                if (++Blink_Device->Blink_State.count >= Blink_Device->Blink_State.repeats) { //已执行次数加1 ，判断是否已经到达执行次数阈值
                    Blink_Device->Blink_State.OnTime = 0; //关闭
                    Blink_Device->Blink_Cfg.ioctrl(false); //关闭
                    Blink_Device->Blink_State.State = false; //关闭
                }
            }
        }

        Blink_Device = Blink_Device->Blink_next;
    }
}

/**
 * @brief 设置Blink设备
 * 
 * @param Name Blink设备名
 * @param OnTime 开启时间(一个单位与Tick有关)
 * @param OffTime 关闭时间(一个单位与Tick有关)
 * @param repeats 重复次数
 */
void BlinkDevice_Config(const char *Name, uint16_t OnTime, uint16_t OffTime, uint8_t repeats) {
    //如果从未创建过设备，退出
    if(Blink_head == NULL) return ;

    //未输入设备名，退出
    if(strlen((const char *)Name) == 0) return ;

    Blink_Info *Blink_Device = Blink_head;
    while(Blink_Device != NULL) {
        if(strcmp((const char *)Blink_Device->Name, (const char *)Name) == 0) { //设备名匹配
            Blink_Device->Blink_State.OnTime  = OnTime;
            Blink_Device->Blink_State.OffTime = OffTime + OnTime;                  
            Blink_Device->Blink_State.repeats = repeats;
            Blink_Device->Blink_State.tick    = Blink_Device->Blink_Cfg.Get_tick();
            Blink_Device->Blink_State.count   = 0;
            if (OnTime  == 0) {
                Blink_Device->Blink_Cfg.ioctrl(false);
                Blink_Device->Blink_State.State  = false;
            } 
            break;
        }
        Blink_Device = Blink_Device->Blink_next;
    }
}   

void BlinkDevice_Show(uint16_t ms) {
    if(Blink_head == NULL) return ;
    Blink_Info *Blink_Device = Blink_head;
    printf("Task ms:%d\r\n",ms);
    printf("=========================================\r\n");
    printf("| Name\t| On\t| Off\t| Rep\t| State\t|\r\n");

    while(Blink_Device != NULL) {
        printf("| %s\t| %d\t| %d\t| %d\t| %s\t|\r\n", Blink_Device->Name, 
                                                        Blink_Device->Blink_State.OnTime, 
                                                        Blink_Device->Blink_State.OffTime, 
                                                        Blink_Device->Blink_State.repeats, 
                                                        Blink_Device->Blink_State.State?"ON":"OFF");
        Blink_Device = Blink_Device->Blink_next;
    }
    printf("=========================================\r\n");
}

#undef true
#undef false