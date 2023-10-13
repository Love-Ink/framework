#ifndef __FRAMEWORK_BLINK_H
#define __FRAMEWORK_BLINK_H

#include <stdio.h>

#define true 1
#define false 0




//Blink初始化函数
typedef struct {

    void (*ioctrl)(uint8_t state);  //操作函数
    uint32_t (*Get_tick)(void);

}Blink_InitTypedef;

//Blink状态函数
typedef struct {
    uint16_t OnTime;    //开启时间
    uint16_t OffTime;   //关闭时间
    uint8_t  repeats;   //重复次数
    uint8_t  State; //状态 开启还是关闭
    uint8_t  count; //计数
    uint32_t  tick;  //当前时间刻
}Blink_state;

//Blink链表函数
typedef struct Blink_Info{
    uint8_t Name[20]; //Blink设备名

    Blink_InitTypedef Blink_Cfg;
    Blink_state       Blink_State;

    struct Blink_Info *Blink_next;
}Blink_Info;

void BlinkDevice_Scan(uint16_t ms);
void BlinkDevice_Creat(const char *Name, void(*ioctrl)(uint8_t state), uint32_t (*Get_tick)(void));
void BlinkDevice_Config(const char *Name, uint16_t OnTime, uint16_t OffTime, uint8_t repeats);
void BlinkDevice_Show(uint16_t ms);
#endif