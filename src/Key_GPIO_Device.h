#ifndef __FRAMEWORCK_KEY_DEVICE_H
#define __FRAMEWORCK_KEY_DEVICE_H

#include "ch32v20x.h"

typedef enum{
    No_Process,         //未按下
    Short_Process,      //短按
    Long_Process        //长按
    // Long_Process_Up ,   //长按后, 按键已松开
    // Long_Process_Down   //长按中, 按键未松开
}FrameWork_ProcessTypes_Index;

typedef enum{
    Key_Up,
    Key_Down
}Key_State;

typedef struct {
    uint8_t     Polarity;       //按键极性，按下是高电平还是低电平
    uint16_t    Long_Process_time_set;     //长按判断时间
    uint8_t (*pGetKeyState_Func)(void);    //按键值获取函数
    uint8_t (*pProcess_Func)(FrameWork_ProcessTypes_Index Process_Types); //按键操作后自动执行

}CreatKey_InitTypedef, FrameWorkKey_Config;

typedef struct {
    Key_State   Key_state;    //按键状态
    FrameWork_ProcessTypes_Index Process_Types; //按键长短按识别结果
    uint16_t    KeyState_Gather;                //按键状态集(判断按键是否按下)
    uint16_t    Long_Process_time_count;        //长按累计时间
}FrameWorkKey_State;

typedef struct FrameWorkKey_Info{

    FrameWorkKey_Config FrameWorkKey_Cfg;
    FrameWorkKey_State   FrameWorkKey_State;

    struct FrameWorkKey_Info *pNext;
}FrameWorkKey_Info;

void Creat_Key(uint8_t Polarity, uint16_t Long_Process_time_set,uint8_t (*pGetKeyState_Func)(void), uint8_t (*pProcess_Func)(FrameWork_ProcessTypes_Index Process_Types)); //创建按键
void Key_Scan(uint16_t ms); //按键扫描

#endif