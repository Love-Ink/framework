#include "TimeSilce_polling.h"
// #include "SystemTick.h"
#include "stdlib.h"
#include "string.h"

//此处请填写当前时钟滴答定时器设定的时间
#define _SYSTEM_TICK_MS     (10) //ms

static Framework_Task_Info *Task_head = NULL;
static uint32_t Task_tick = 0;
static uint8_t TaskRemarks_Running = 0;
static uint8_t Task_count = 0;

static Framework_Task_Info *Task_Delete(Framework_Task_Info *Task);

/**
 * @brief 创建 "时间片轮询"  任务
 * 
 * @param Task_ID 任务ID号
 * @param Time 运行间隔时间(一个单位为10ms)
 * @param TaskHook 任务函数
 */
void CreatTask_Func(uint8_t Task_ID, uint32_t Time, void (*TaskHook)(uint16_t ms)) 
{
    // SysTick_CTLR->STIE = 0;
    if(TaskHook == NULL) return ;

    Framework_Task_Info *Creat_Task = malloc(sizeof(Framework_Task_Info));
    memset(Creat_Task, 0, sizeof(Framework_Task_Info)); //初始化保险点，不然链表中 next 变量不是NULL

    if(Time < 10) Time = 10;
    Creat_Task->Task_Cfg.Time = Time / _SYSTEM_TICK_MS;
    Creat_Task->Task_Cfg.ItvTime = Time / _SYSTEM_TICK_MS;
    Creat_Task->Task_Cfg.TaskHook = TaskHook;
    Creat_Task->Task_ID = Task_ID;
    Creat_Task->NewState = Task_State_Run;
    if(Task_head == NULL) {
        Task_head = Creat_Task;
        Task_count ++;
    } else {
        Framework_Task_Info *Temp = Task_head;
        while(Temp->Task_next != NULL) Temp = Temp->Task_next;
        Temp->Task_next = Creat_Task; //下一个
        Creat_Task->Task_back = Temp; //前一个
        Task_count ++;
    }
    // SysTick_CTLR->STIE = 1;
    // Application_SysTick_SR_Clear();

}

/**
 * @brief 任务标志处理
 * @property 放入中断中
 */
void TaskRemarks(void) 
{
    if(Task_head == NULL) return ;
    TaskRemarks_Running = 1;
    Framework_Task_Info *Task = Task_head;

    while(Task != NULL) {
        if(/*(Task->Task_Cfg.ItvTime) &&*/ (Task->NewState == Task_State_Run)) { //当前任务非挂起状态
            // printf("Time:%d,%d\r\n", Task->Task_Cfg.Time, Task_count);
            if (Task->Task_Cfg.Time <= Task->Task_Cfg.ItvTime) { //当任务计数合法，正常倒计时。
                Task->Task_Cfg.Time --;
            } else { //当前任务计数不合法，初始化。
                Task->Task_Cfg.Time = Task->Task_Cfg.ItvTime;
            }

            if(Task->Task_Cfg.Time == 0) { //任务间隔时间为0，允许执行任务
                Task->Task_Cfg.Time = Task->Task_Cfg.ItvTime;
                Task->Task_Cfg.Run = 1;
            }
        }
        Task = Task->Task_next;
    }
    Task_tick ++;
    TaskRemarks_Running = 0;
}

/**
 * @brief 任务处理
 * @property 放入main函数中
 */
void TaskProcess(void) 
{
    if(Task_head == NULL) return ;

    Framework_Task_Info *Task = Task_head;
    
    while(Task != NULL) {
        if(Task->NewState != Task_State_Run) {Task = Task->Task_next; continue;}
        if(Task->Task_Cfg.Run) {
            Task->Task_Cfg.TaskHook(Task->Task_Cfg.ItvTime * _SYSTEM_TICK_MS);//一个单位10ms
            Task->Task_Cfg.Run = 0;
            if(Task->Task_Cfg.Run_Count > 0) {
                Task->Task_Cfg.Run_Count --;
                if(Task->Task_Cfg.Run_Count == 0) {
                    if(Task->Task_Cfg.Delete_TaskHook != NULL) Task->Task_Cfg.Delete_TaskHook();
                    Task = Task_Delete(Task);
                    continue;
                }
            }
        }
        Task = Task->Task_next;
    }
}

/**
 * @brief 复位距下一次调度时间
 * 
 * @param Task_ID 
 */
void Task_Reset_Time(uint8_t Task_ID) {
    if(Task_head == NULL) return ;
    Framework_Task_Info *Task = Task_head;
    
    while(Task != NULL) {
        if(Task->Task_ID == Task_ID) {
            Task->Task_Cfg.Time =Task->Task_Cfg.ItvTime;
            break;
        }
        Task = Task->Task_next; 
    }
}

/**
 * @brief 设置任务时间量
 * 
 * @param Task_ID 任务ID
 * @param Time 距下一次调度还有 (ms)
 * @param ItvTime 任务调度时间 (ms)
 */
void Task_Cfg_Time(uint8_t Task_ID, uint32_t Time, uint32_t ItvTime) {
    if(Task_head == NULL) return ;
    Framework_Task_Info *Task = Task_head;
    if(Time < 10) Time = 10;
    if(ItvTime < 10) ItvTime = 10;
    
    while(Task != NULL) {
        if(Task->Task_ID == Task_ID) {
            Task->Task_Cfg.Time = Time / _SYSTEM_TICK_MS;
            Task->Task_Cfg.ItvTime = ItvTime / _SYSTEM_TICK_MS;
            break;
        }
        Task = Task->Task_next; 
    }
}

/**
 * @brief 获取任务剩余调度时间
 * 
 * @param Task_ID 任务ID
 * @return uint32_t 距下一次调度还有(ms)
 */
uint32_t Task_Get_ResidueTime(uint8_t Task_ID) {
    if(Task_head == NULL) return 0;
    Framework_Task_Info *Task = Task_head;
    
    while(Task != NULL) {
        if(Task->Task_ID == Task_ID) {
            return Task->Task_Cfg.Time;
        }
        Task = Task->Task_next; 
    }
    return 0;
}

/**
 * @brief 配置任务执行状态
 * 
 * @param Task_ID 任务ID
 * @param Task_state_ 任务状态（详见 Task_state）
 */
void Task_Suspend(uint8_t Task_ID ,Task_state Task_state_) {
    if(Task_head == NULL) return ;
    
    Framework_Task_Info *Task = Task_head;
    
    while(Task != NULL) {
        if(Task->Task_ID == Task_ID) {
            Task->NewState = Task_state_;
            break;
        }
        Task = Task->Task_next; 
    }
}

/**
 * @brief 设置任务寿命
 * 
 * @param Task_ID 任务ID
 * @param Run_Count 剩余任务执行次数
 * @param Delete_TaskHook 收尾人任务
 */
void Task_Cfg_Life(uint8_t Task_ID, uint16_t Run_Count, void(*Delete_TaskHook)(void)) {
    if(Task_head == NULL) return ;
    
    Framework_Task_Info *Task = Task_head;
    
    while(Task != NULL) {
        if(Task->Task_ID == Task_ID) {
            Task->Task_Cfg.Run_Count = Run_Count;
            Task->Task_Cfg.Delete_TaskHook = Delete_TaskHook;
            break;
        }
        Task = Task->Task_next; 
    }
}

/**
 * @brief 任务删除（不允许用户调用，怕出事）
 * 
 * @param Task 任务链表
 * @return Framework_Task_Info* 下一个任务链表地址
 */
static Framework_Task_Info *Task_Delete(Framework_Task_Info *Task) {
    while(TaskRemarks_Running);
    //  SysTick_CTLR->STIE = 0;
    if(Task == NULL) return NULL;
    if(Task_count <= 1) return Task;
    
    if(Task->Task_back != NULL) {
        Task->Task_back->Task_next = Task->Task_next;
        Task->Task_next->Task_back = Task->Task_back;
    } else {
        Task_head = Task->Task_next;
        
        Task->Task_next->Task_back = NULL;
    }

    Framework_Task_Info *ret = Task->Task_next;

    free(Task);
    Task_count --;

    //  SysTick_CTLR->STIE = 1;
    //  Application_SysTick_SR_Clear();
    return ret;
}

/**
 * @brief 获取Systick 计数
 * 
 * @return uint32_t Systick 计数
 */
uint32_t Get_TaskTick() {
    return Task_tick;
}

/**
 * @brief 获取当前创建任务数量
 * 
 * @return uint8_t 当前创建任务数量
 */
uint8_t Get_TaskCount() {
    return Task_count;
}

/**
 * @brief 显示当前任务详细信息
 * 
 * @param ms 调度时间(ms)
 */
void Task_show(uint16_t ms){
    if(Task_head == NULL) return ;

    Framework_Task_Info *Task = Task_head;
    
    printf("=========================================================================\r\n");
    printf("| Task_ID\t| ItvTime(ms)\t| Run_Count\t| Time\t| Task_State\t|\r\n");
    while(Task != NULL) {
        printf("| %d\t\t| %ld\t\t| %d\t\t| %ld\t| %s\t|\r\n", Task->Task_ID, 
                                                        Task->Task_Cfg.ItvTime * _SYSTEM_TICK_MS, 
                                                        Task->Task_Cfg.Run_Count, 
                                                        Task->Task_Cfg.Time * _SYSTEM_TICK_MS, 
                                                        Task->NewState?"Task_Supend":"Task_Run");
        Task = Task->Task_next; 
    }
    printf("=========================================================================\r\n");
}

#undef _SYSTEM_TICK_MS
