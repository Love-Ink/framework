#ifndef __FRAMEWORK_TIMESILECE_POLLING_H
#define __FRAMEWORK_TIMESILECE_POLLING_H

#include "stdio.h"


typedef enum {
    Task_State_Run,     //任务状态为运行
    Task_State_Suspend  //任务状态为挂起
}Task_state;


typedef struct {
    uint8_t Run;
    uint16_t Run_Count;
    uint32_t Time;
    uint32_t ItvTime;
    void (*TaskHook)(uint16_t ms);
    void (*Delete_TaskHook)(void);
}CreatTask_InitTypedef;

typedef struct Framework_Task_Info{
    uint8_t Task_ID;
    Task_state NewState;

    CreatTask_InitTypedef Task_Cfg;
    struct Framework_Task_Info *Task_back;
    struct Framework_Task_Info *Task_next;

}Framework_Task_Info;

void CreatTask_Func(uint8_t Task_ID, uint32_t Time, void (*TaskHook)(uint16_t ms)); //创建时间片轮询任务
void TaskRemarks(void); //任务标志位置位
void TaskProcess(void); //任务执行函数
uint32_t Get_TaskTick();    //获取任务时间刻 每个单位按SysTick 系统滴答定时器的时间为定，当前为10ms每单位
uint8_t Get_TaskCount();    //获取当前任务总数
void Task_Suspend(uint8_t Task_ID ,Task_state NewState); //任务挂起
void Task_Cfg_Life(uint8_t Task_ID, uint16_t Run_Count, void(*Delete_TaskHook)(void));  //设置任务寿命
void Task_Cfg_Time(uint8_t Task_ID, uint32_t Time, uint32_t ItvTime);   //修改任务轮询时间
void Task_show(uint16_t ms);    //显示当前所有任务状态 


#endif