#include "ConfigParameters.h"
#include <stdio.h>
#include <assert.h>
static ConfigParameters_Info* CfgParame_Head = NULL;
static ConfigParameters_Info* CfgParame_Now = NULL;

#ifdef _DEBUG
#define ASSERT(Condition,arg)      \
 if(Condition) {\
     printf("[%s:%d]:%s\r\n", __FILE__, __LINE__, arg);\
     assert(0);\
 }
#else
#define ASSERT(Condition,arg)     
#endif
/**
 * @brief 参数配置项创建
 *
 * @param CfgParame_ID 配置参数ID号
 * @param Val 配置参数指针
 * @param Val_Type 配置参数类型
 * @param Max 最大配置
 * @param Min 最小配置
 */
void ConfigParameters_Creat(uint16_t CfgParame_ID, void* Val, Val_Type_enum Val_Type, int32_t Max, int32_t Min)
{
    ConfigParameters_Info* CfgParame_new = malloc(sizeof(ConfigParameters_Info));  //分配一个新链表
    ASSERT((CfgParame_new == NULL), "malloc failed");
    memset(CfgParame_new, 0, sizeof(ConfigParameters_Info)); //初始化链表内容 重要

    ASSERT((Val == NULL), "Create p_Val is NULL");
    CfgParame_new->CfgParam_Cfg.Val = Val;
    CfgParame_new->CfgParam_Cfg.Max = Max;
    CfgParame_new->CfgParam_Cfg.Min = Min;
    CfgParame_new->CfgParam_Cfg.Val_Type = Val_Type;
    CfgParame_new->CfgParame_ID = CfgParame_ID;

    if (CfgParame_Head == NULL) { //是否为头
        CfgParame_Head = CfgParame_new;
        CfgParame_Now = CfgParame_Head;
    }
    else {
        ConfigParameters_Info* CfgParame_Info = CfgParame_Head;

        while (CfgParame_Info->CfgParam_next != NULL)  CfgParame_Info = CfgParame_Info->CfgParam_next;
        CfgParame_Info->CfgParam_next = CfgParame_new;
    }
}

/**
 * @brief 切换配置参数
 *
 * @return 返回下一个配置参数值指针
 */
void* ConfigParameters_OrderSwitch(void)
{

    if (CfgParame_Now->CfgParam_next != NULL) {
        CfgParame_Now = CfgParame_Now->CfgParam_next;
    }
    else {
        CfgParame_Now = CfgParame_Head;
    }

    ASSERT((CfgParame_Now->CfgParam_Cfg.Val == NULL), "p_Val is NULL!");
    return CfgParame_Now->CfgParam_Cfg.Val;
}

/**
 * @brief 配置参数 加
 *
 * @param Scale 加数
 * @return 返回配置参数值指针
 */
void* ConfigParameters_PlusVal(uint64_t Scale)
{
    ASSERT((CfgParame_Head == NULL), "CfgParame_Head is NULL, Please Create ConfigParameter!");
    if (CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    switch (CfgParame_Now->CfgParam_Cfg.Val_Type) {
    case TYPE_INT8: {
        if ((*(int8_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (int8_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val + (int8_t)Scale;
        }
        else {
            *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_UINT8: {
        if ((*(uint8_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (uint8_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val = (*(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val) + ((uint8_t)Scale);
        }
        else {
            *(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_INT16: {
        if ((*(int16_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (int16_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val + (int16_t)Scale;
        }
        else {
            *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_UINT16: {
        if ((*(uint16_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (uint16_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val + (uint16_t)Scale;
        }
        else {
            *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_INT32: {
        if ((*(int32_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (int32_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val + (int32_t)Scale;
        }
        else {
            *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_UINT32: {
        if ((*(uint32_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (uint32_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val + (uint32_t)Scale;
        }
        else {
            *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_INT64: {
        if ((*(int64_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (int64_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val + (int64_t)Scale;
        }
        else {
            *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_UINT64: {
        if ((*(uint64_t*)(CfgParame_Now->CfgParam_Cfg.Val) + (uint64_t)Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val + (uint64_t)Scale;
        }
        else {
            *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_FLOAT: {
        float F_Scale = 0.0;
        memcpy(&F_Scale, &Scale, sizeof(float));
        if ((*(float*)(CfgParame_Now->CfgParam_Cfg.Val) + F_Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(float*)CfgParame_Now->CfgParam_Cfg.Val = *(float*)CfgParame_Now->CfgParam_Cfg.Val + F_Scale;
        }
        else {
            *(float*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    case TYPE_DOUBLE: {
        double D_Scale = 0.0;
        memcpy(&D_Scale, &Scale, sizeof(double));
        if ((*(double*)(CfgParame_Now->CfgParam_Cfg.Val) + D_Scale) <= CfgParame_Now->CfgParam_Cfg.Max) {
            *(double*)CfgParame_Now->CfgParam_Cfg.Val = *(double*)CfgParame_Now->CfgParam_Cfg.Val + D_Scale;
        }
        else {
            *(double*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Max;
        }
    }break;

    default: {

    }break;
    }

    ASSERT((CfgParame_Now->CfgParam_Cfg.Val == NULL), "p_Val is NULL!");
    return CfgParame_Now->CfgParam_Cfg.Val;
}

/**
 * @brief 配置参数 减
 *
 * @param Scale 减数
 * @return 返回配置参数值指针
 */
void* ConfigParameters_ReduceVal(uint64_t Scale)
{
    ASSERT((CfgParame_Head == NULL), "CfgParame_Head is NULL, Please Create ConfigParameter!");
    if (CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    switch (CfgParame_Now->CfgParam_Cfg.Val_Type) {
    case TYPE_INT8: {
        if ((*(int8_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (int8_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val - (int8_t)Scale;
        }
        else {
            *(int8_t*)CfgParame_Now->CfgParam_Cfg.Val = (int8_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_UINT8: {
        if ((*(uint8_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (uint8_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val - (uint8_t)Scale;
        }
        else {
            *(uint8_t*)CfgParame_Now->CfgParam_Cfg.Val = (uint8_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_INT16: {
        if ((*(int16_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (int16_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val - (int16_t)Scale;
        }
        else {
            *(int16_t*)CfgParame_Now->CfgParam_Cfg.Val = (int16_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_UINT16: {
        if ((*(uint16_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (uint16_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val - (uint16_t)Scale;
        }
        else {
            *(uint16_t*)CfgParame_Now->CfgParam_Cfg.Val = (uint16_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_INT32: {
        if ((*(int32_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (int32_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val - (int32_t)Scale;
        }
        else {
            *(int32_t*)CfgParame_Now->CfgParam_Cfg.Val = CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_UINT32: {
        if ((*(uint32_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (uint32_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val - (uint32_t)Scale;
        }
        else {
            *(uint32_t*)CfgParame_Now->CfgParam_Cfg.Val = (uint32_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_INT64: {
        if ((*(int64_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (int64_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val = *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val - (int64_t)Scale;
        }
        else {
            *(int64_t*)CfgParame_Now->CfgParam_Cfg.Val = (int64_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_UINT64: {
        if ((*(uint64_t*)(CfgParame_Now->CfgParam_Cfg.Val) - (uint64_t)Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val = *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val - (uint64_t)Scale;
        }
        else {
            *(uint64_t*)CfgParame_Now->CfgParam_Cfg.Val = (uint64_t)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_FLOAT: {
        float F_Scale = 0.0;
        memcpy(&F_Scale, &Scale, sizeof(float));
        if ((*(float*)(CfgParame_Now->CfgParam_Cfg.Val) - F_Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(float*)CfgParame_Now->CfgParam_Cfg.Val = *(float*)CfgParame_Now->CfgParam_Cfg.Val - F_Scale;
        }
        else {
            *(float*)CfgParame_Now->CfgParam_Cfg.Val = (float)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    case TYPE_DOUBLE: {
        double D_Scale = 0.0;
        memcpy(&D_Scale, &Scale, sizeof(double));
        if ((*(double*)(CfgParame_Now->CfgParam_Cfg.Val) - D_Scale) >= CfgParame_Now->CfgParam_Cfg.Min) {
            *(double*)CfgParame_Now->CfgParam_Cfg.Val = *(double*)CfgParame_Now->CfgParam_Cfg.Val - D_Scale;
        }
        else {
            *(double*)CfgParame_Now->CfgParam_Cfg.Val = (double)CfgParame_Now->CfgParam_Cfg.Min;
        }
    }break;

    default: {

    }break;
    }

    ASSERT((CfgParame_Now->CfgParam_Cfg.Val == NULL), "p_Val is NULL!");
    return CfgParame_Now->CfgParam_Cfg.Val;
}

/**
 * @brief 获取当前配置参数值
 *
 * @return 返回配置参数值指针
 */
void* ConfigParameters_GetVal(void)
{
    ASSERT((CfgParame_Head == NULL), "CfgParame_Head is NULL, Please Create ConfigParameter!");
    if (CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    ASSERT((CfgParame_Now->CfgParam_Cfg.Val == NULL), "p_Val is NULL!");
    return CfgParame_Now->CfgParam_Cfg.Val;
}

/**
 * @brief 获取当前配置ID
 *
 * @return uint16_t 配置ID
 */
int16_t ConfigParameters_GetID(void)
{
    ASSERT((CfgParame_Head == NULL), "CfgParame_Head is NULL, Please Create ConfigParameter!");
    if (CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    return ((int16_t)CfgParame_Now->CfgParame_ID);
}

/**
 * @brief 返回配置参数头
 *
 */
void ConfigParameters_GoHeadPage(void)
{
    ASSERT((CfgParame_Head == NULL), "CfgParame_Head is NULL, Please Create ConfigParameter!");
    if (CfgParame_Now == NULL) CfgParame_Now = CfgParame_Head;

    CfgParame_Now = CfgParame_Head;
}

/**
 * @brief Float 转 UINT64 (二进制)
 * 
 */
uint64_t Float_to_uint64(float num) 
{
    uint64_t ret = 0;

    memcpy(&ret, &num, sizeof(float));

    return ret;
}

#undef ASSERT
