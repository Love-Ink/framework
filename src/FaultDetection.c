#include "FaultDetection.h"
#include "string.h"
#include "stdlib.h"

static FaultDetection_Info *FaultDetection_Head = NULL;

static uint8_t FaultDetection_Count = 0;

/**
 * @brief 创建故障监测
 *  
 * @param name 故障监测项名
 * @param Init 初始化结构体
 * @param NewState 是否使能此项故障监测
 */
void Creat_FaultDetection(char *name, FalutDetection_Init *Init, FunctionalState NewState){
    if(Init == NULL) return ;

    FaultDetection_Info *FaultDet_new = malloc(sizeof(FaultDetection_Info));
    memset(FaultDet_new, 0, sizeof(FaultDetection_Info));

    memcpy(&FaultDet_new->FalDet_Cfg, Init, sizeof(FalutDetection_Init));
    
    if(strlen((const char *)name) == 0) {
        sprintf((char *)&FaultDet_new->FalDet_Name, "FalDet_%d", FaultDetection_Count);
    } else {
        memcpy(FaultDet_new->FalDet_Name, name, strlen((const char *)name));
    }

    FaultDet_new->NewState = NewState;

    if(FaultDetection_Head == NULL) {
        FaultDetection_Head = FaultDet_new;
        FaultDetection_Count ++;
    } else {
        FaultDetection_Info *Fault_Info = FaultDetection_Head;

        while(Fault_Info->FalDet_next != NULL)  Fault_Info = Fault_Info->FalDet_next;

        Fault_Info->FalDet_next = FaultDet_new;
        FaultDetection_Count ++;
    }
}

//故障判断 
void FaultDet_Compare(uint16_t ms){
    if(FaultDetection_Head == NULL) return ;

    FaultDetection_Info *Fault_Info = FaultDetection_Head;

    while(Fault_Info != NULL) {    //遍历链表

        FalutDetection_Init *Fault_Cfg = &Fault_Info->FalDet_Cfg;
        struct FalutDetection_State *Fault_State = &Fault_Info->FalDet_State;
        uint32_t Threshold = 0;
        Threshold = (Fault_Cfg->pThreshold == NULL) ? (Fault_Cfg->Threshold) : (*Fault_Cfg->pThreshold);
        switch(Fault_Cfg->Relation) {
            case Less_than:                 //小于
                if(*Fault_Cfg->pDet_val <  Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            case Less_than_or_Equal_to:     //小于等于
                if(*Fault_Cfg->pDet_val <= Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            case Greater_than:              //大于
                if(*Fault_Cfg->pDet_val >  Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            case Greater_than_or_Equal_to:  //大于等于
                if(*Fault_Cfg->pDet_val >= Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            case Equal_to:                  //等于
                if(*Fault_Cfg->pDet_val == Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            case Not_Equal_to:              //不等于
                if(*Fault_Cfg->pDet_val != Threshold) Fault_State->Threshold_Flag= 1;
                else {Fault_State->Threshold_Flag = 0;}
                break;
            default:break;
        }
        Fault_Info = Fault_Info->FalDet_next;
    }
}

/**
 * @brief 故障计数
 * 
 * @param ms 故障计数函数进入间隔
 */
void FaultDet_Timecount(uint16_t ms){
    if(FaultDetection_Head == NULL) return ;

    FaultDetection_Info *Fault_Info = FaultDetection_Head;
    while(Fault_Info != NULL) {    //遍历链表
        FalutDetection_Init *Fault_Cfg = &Fault_Info->FalDet_Cfg;
        struct FalutDetection_State *Fault_State = &Fault_Info->FalDet_State;
        if(Fault_Info->NewState != ENABLE) {Fault_Info = Fault_Info->FalDet_next; continue;}
        if(Fault_State->Threshold_Flag == 1) { //故障了
            Fault_State->Hold_time_count += ms; //故障维持时间加加
            if(Fault_State->Hold_time_count >= Fault_Cfg->Hold_time_set) { //时间到
                Fault_State->Hold_time_up_Flag = 1; //置位时间到标志位
            }
        } else {
            if(Fault_Cfg->AutoRemove_Fault == 1) { //自动移除故障功能，当故障解除自动消除置位
                Fault_State->Hold_time_up_Flag = 0;
                Fault_State->Hold_time_count = 0;
            } else {
                //手动消除故障
            }
            if(Fault_State->Hold_time_up_Flag != 1) {   //故障时间维持短，没达到设置时间自动清除
                Fault_State->Hold_time_count = 0;
            }
        }
        Fault_Info = Fault_Info->FalDet_next;
    }
    return ;
}

//故障响应
void FaultDet_Func(uint16_t ms) {
    if(FaultDetection_Head == NULL) return ;

    FaultDetection_Info *Fault_Info = FaultDetection_Head;
    while(Fault_Info != NULL) {    //遍历链表
        FalutDetection_Init *Fault_Cfg = &Fault_Info->FalDet_Cfg;
        struct FalutDetection_State *Fault_State = &Fault_Info->FalDet_State;
        if(Fault_Info->NewState != ENABLE) {Fault_Info = Fault_Info->FalDet_next; continue;}
        if(Fault_State->Hold_time_up_Flag != Fault_State->Hold_time_up_Flag_back) { //状态改变
            Fault_Cfg->Func(Fault_State->Hold_time_up_Flag);
            Fault_State->Hold_time_up_Flag_back = Fault_State->Hold_time_up_Flag;
        }
        Fault_Info = Fault_Info->FalDet_next; //指向下一个链表
    }
}

//移除故障标志
void Remove_FaultDetflag(char *name){
        if(FaultDetection_Head == NULL) return ;

    FaultDetection_Info *Fault_Info = FaultDetection_Head;
    while(Fault_Info != NULL) {    //遍历链表
        struct FalutDetection_State *Fault_State = &Fault_Info->FalDet_State;
        
        if(strcmp((const char *)name, (const char *)&Fault_Info->FalDet_Name) == 0) {
            Fault_State->Hold_time_up_Flag = 0;
            Fault_State->Hold_time_count = 0;
            return ;
        }
        Fault_Info = Fault_Info->FalDet_next; //指向下一个链表
    }
}

void FaultDat_Config_State(char *name, FunctionalState NewState){
        if(FaultDetection_Head == NULL) return ;

    FaultDetection_Info *Fault_Info = FaultDetection_Head;
    while(Fault_Info != NULL) {    //遍历链表
        
        if(strcmp((const char *)name, (const char *)&Fault_Info->FalDet_Name) == 0) {
            Fault_Info->NewState = NewState;
            return ;
        }
        Fault_Info = Fault_Info->FalDet_next; //指向下一个链表
    }
}

/**
 * @brief  显示所有注册监测的各项信息
 * 
 * @param ms 时间量
 */
void Fault_show(uint16_t ms){
    if(FaultDetection_Head == NULL) return ;

    printf("FaultDetection_Count:%d\r\n",FaultDetection_Count);
    struct FaultDetection_Info *FaultDet = FaultDetection_Head;
    printf("=================================================================\r\n");
    printf("| Name\t\t| Threshold\t| Hold_time_set\t| Auto_Remove\t|\r\n");
    while(FaultDet != NULL) {
        if(&FaultDet->FalDet_Cfg == NULL) {FaultDet = FaultDet->FalDet_next; continue ;}
        printf("| %s\t| %ld\t\t| %d\t\t| %s\t|\r\n",FaultDet->FalDet_Name, \
                                            FaultDet->FalDet_Cfg.Threshold, \
                                            FaultDet->FalDet_Cfg.Hold_time_set, \
                                            FaultDet->FalDet_Cfg.AutoRemove_Fault?"ENABLE":"DISABLE");
        FaultDet = FaultDet->FalDet_next;
    }
    printf("=================================================================\r\n");
}

/**
 * @brief 显示当前超过阈值的监测项信息
 * 
 * @param ms 
 */
void Fault_INFO_show(uint16_t ms){
    if(FaultDetection_Head == NULL) return ;

    printf("FaultDetection_Count:%d\r\n",FaultDetection_Count);
    struct FaultDetection_Info *FaultDet = FaultDetection_Head;
    printf("==============================================================================\r\n");
    printf("| Name\t\t| Fault_flag\t| Hold_time_count\t| Fault_Val\t|\r\n");
    while(FaultDet != NULL) {
        if(FaultDet->FalDet_State.Threshold_Flag == 1 || FaultDet->FalDet_State.Hold_time_up_Flag == 1)
            printf("| %s\t| %d\t\t| %d\t\t\t| %ld\t\t|\r\n",FaultDet->FalDet_Name, \
                                                                FaultDet->FalDet_State.Threshold_Flag, \
                                                                FaultDet->FalDet_State.Hold_time_count, \
                                                                *FaultDet->FalDet_Cfg.pDet_val);
        FaultDet = FaultDet->FalDet_next;
    }
    printf("==============================================================================\r\n");
}