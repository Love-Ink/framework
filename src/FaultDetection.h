#ifndef APPLICATION_FAULT_DETECTION_FAULTDETECTION_H_
#define APPLICATION_FAULT_DETECTION_FAULTDETECTION_H_

#include "stdio.h"

#ifdef __CORE_RISCV_H__

#else
    typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#endif

typedef enum{
    Less_than,                  //小于
    Less_than_or_Equal_to,      //小于等于
    Greater_than,               //大于
    Greater_than_or_Equal_to,   //大于等于
    Equal_to,                   //等于
    Not_Equal_to                //不等于

}Relation_enum;

typedef struct FalutDetection_Config{
    int32_t        *pDet_val;          //监测变量地址
    uint32_t        *pThreshold;        //会变化的阈值
    uint32_t        Threshold;           //针对宏阈值(不变化的阈值)
    uint16_t        Hold_time_set;      //保持多长时间触发(ms)
    Relation_enum   Relation;           //关系

    void (*Func)(uint16_t ); //出现故障自动进入此函数处理

    FunctionalState AutoRemove_Fault;   //自动消除故障

}FalutDetection_Init;

struct FalutDetection_State{
    uint8_t     Threshold_Flag;     //达到阈值标志位
    uint8_t     Hold_time_up_Flag;  //达到设定时间标志位    
    uint8_t     Hold_time_up_Flag_back;//达到设定时间标志位（上一次）
    uint16_t    Hold_time_count;    //维持时间计数
};

typedef struct FaultDetection_Info{
    FunctionalState     NewState;
    uint8_t FalDet_Name[20];

    struct FalutDetection_Config    FalDet_Cfg;
    struct FalutDetection_State     FalDet_State;

    struct FaultDetection_Info      *FalDet_next;
}FaultDetection_Info;

#ifdef __cplusplus 
extern "C" {
#endif

void Fault_show(uint16_t ms);
void Fault_INFO_show(uint16_t ms);
void FaultDet_Func(uint16_t ms); //故障响应
void FaultDet_Compare(uint16_t ms); //故障判断
void FaultDet_Timecount(uint16_t ms); //故障计数
void Remove_FaultDetflag(char *name); //移除故障标志
void FaultDat_Config_State(char *name, FunctionalState NewState); //故障检测是否使能
void Creat_FaultDetection(char *name, FalutDetection_Init *Init, FunctionalState NewState); //创建故障监测

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_FAULT_DETECTION_FAULTDETECTION_H_ */
