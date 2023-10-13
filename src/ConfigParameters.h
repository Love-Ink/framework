#ifndef __FRAMEWORK_CONFIG_PARAMETERS_H
#define __FRAMEWORK_CONFIG_PARAMETERS_H

#include "stdlib.h"
#include "string.h"
#include "stdint.h"


typedef enum {
    TYPE_INT8 = 0,
    TYPE_UINT8,

    TYPE_INT16,
    TYPE_UINT16,

    TYPE_INT32,
    TYPE_UINT32,

    TYPE_INT64,
    TYPE_UINT64,

    TYPE_FLOAT,
    TYPE_DOUBLE,
}Val_Type_enum;


typedef struct ConfigParameters_Config {
    void       *Val;    //配置参数指针
    int32_t    Max;     //设置最大值  
    int32_t    Min;     //设置最小值
    uint8_t    Val_Type;//配置参数类型
}ConfigParameters_Init;

typedef struct ConfigParameters_Info {
    uint16_t CfgParame_ID;  //配置参数ID

    ConfigParameters_Init CfgParam_Cfg;
    struct ConfigParameters_Info* CfgParam_next;    //下一个链表
}ConfigParameters_Info;

#ifdef __cplusplus
extern "C" {
#endif

    //创建配置参数项
    void ConfigParameters_Creat(uint16_t CfgParame_ID, void* Val, Val_Type_enum Val_Type, int32_t Max, int32_t Min);

    //顺序切换到下一个参数项		返回当前配置项值指针
    void* ConfigParameters_OrderSwitch(void);

    //对当前参数项加  	返回当前配置项值指针
    void* ConfigParameters_PlusVal(uint64_t Scale);

    //对当前参数项减		返回当前配置项值指针
    void* ConfigParameters_ReduceVal(uint64_t Scale);

    //获取当前配置项值指针
    void* ConfigParameters_GetVal(void);

    //获取当前配置项ID号
    int16_t ConfigParameters_GetID(void);

    //返回最开始的配置项
    void ConfigParameters_GoHeadPage(void);

    //FLOAT 转 UINT64
    uint64_t Float_to_uint64(float num);

#ifdef __cplusplus
}
#endif

#endif
