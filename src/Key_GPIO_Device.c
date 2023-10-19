#include "Key_GPIO_Device.h"
#include "stdlib.h"
#include "string.h"

static FrameWorkKey_Info *Key_Head = NULL;

static uint8_t CreatKey_Count = 0;


/**
 * @brief 创建按键
 * 
 * @param Polarity 按键极性，按下是高电平还是低电平  高电平写(1) 低电平写(0)
 * @param Long_Process_time_set 长按判定时间(ms)
 * @param pGetKeyState_Func 获取按键状态函数指针
 * @param pProcess_Func 当按键有操作自动执行的函数指针
 */
void Creat_Key(uint8_t Polarity, uint16_t Long_Process_time_set,uint8_t (*pGetKeyState_Func)(void), void (*pProcess_Func)(FrameWork_ProcessTypes_Index Process_Types)) {
    if(pGetKeyState_Func == NULL) return ;
    if(pProcess_Func == NULL) return ; 

    FrameWorkKey_Info *pNewCreatKey_Initstructure = malloc(sizeof(FrameWorkKey_Info));
    memset(pNewCreatKey_Initstructure, 0, sizeof(FrameWorkKey_Info));

    pNewCreatKey_Initstructure->FrameWorkKey_Cfg.Polarity = Polarity;
    pNewCreatKey_Initstructure->FrameWorkKey_Cfg.Long_Process_time_set = Long_Process_time_set;
    pNewCreatKey_Initstructure->FrameWorkKey_Cfg.pGetKeyState_Func = pGetKeyState_Func;
    pNewCreatKey_Initstructure->FrameWorkKey_Cfg.pProcess_Func = pProcess_Func;
    
    if(Key_Head == NULL){
        Key_Head = pNewCreatKey_Initstructure;
        CreatKey_Count ++;
    } else {
        FrameWorkKey_Info *pTemp_KeyInitStructure = Key_Head;
        while(pTemp_KeyInitStructure->pNext != NULL) pTemp_KeyInitStructure = pTemp_KeyInitStructure->pNext;

        pTemp_KeyInitStructure->pNext = pNewCreatKey_Initstructure;
        CreatKey_Count ++;
    }
}

/**
 * @brief 按键扫描
 * 
 * @param ms 按键扫描间隔
 */
void Key_Scan(uint16_t ms) {
    FrameWorkKey_Info *FwKey_Info = Key_Head;
    while(FwKey_Info != NULL) { //遍历链表
        FrameWorkKey_Config *FwKey_Cfg = &FwKey_Info->FrameWorkKey_Cfg;
        FrameWorkKey_State *FwKey_State = &FwKey_Info->FrameWorkKey_State;

        for(uint8_t i = 0; i < 4; i++) {
            FwKey_State->KeyState_Gather |= (FwKey_Cfg->pGetKeyState_Func() == FwKey_Cfg->Polarity); //判断按键
            if((FwKey_State->KeyState_Gather & 0x0F) == 0x0F) {
                FwKey_State->Key_state = Key_Down;         //按下了
            } else if((FwKey_State->KeyState_Gather & 0x0F) == 0x00) {
                FwKey_State->Key_state = Key_Up;         //松开了
            } else {
                //其余都是按键抖动
            }

            FwKey_State->KeyState_Gather = (FwKey_State->KeyState_Gather << 1) & 0xFFFF; //左移
        }

        if(FwKey_State->Key_state == Key_Down) { //当按键按下，开始计数
            FwKey_State->Long_Process_time_count += ms;
            if(FwKey_State->Long_Process_time_count >= FwKey_Cfg->Long_Process_time_set) { //超过设定长按时间,判定为长按
                FwKey_State->Process_Types = Long_Process;  //设置为长按
            } else {
                FwKey_State->Process_Types = Short_Process; //设置为短按
            }
        } else { //当按键弹起，开始执行
            if((FwKey_State->Process_Types != No_Process)) {//有按下过，且已经松手
                FwKey_Cfg->pProcess_Func(FwKey_State->Process_Types); //将结果给执行函数
                FwKey_State->Process_Types = No_Process;    //清标志位
                FwKey_State->Long_Process_time_count = 0;   //重新计数
            } else {
                FwKey_State->Long_Process_time_count = 0;   //重新计数
            }
        }
        FwKey_Info = FwKey_Info->pNext; //下一个链表
    }
}


// void Key_Creat_Show(uint16_t ms) {
//     FrameWorkKey_Info *FwKey_Info = Key_Head;
//     while(FwKey_Info != NULL) { //遍历链表

//         FrameWorkKey_Config *FwKey_Cfg = &FwKey_Info->FrameWorkKey_Cfg;
//         FrameWorkKey_State *FwKey_State = &FwKey_Info->FrameWorkKey_State;

//         FwKey_Info = FwKey_Info->pNext; //下一个链表
//     }
// }
