#include "efsm.h"
#include <stdio.h>

// 事件命令定义
#define CMD_START (EFSM_STATE_USER_CMD_BASE + 1)
#define CMD_STOP  (EFSM_STATE_USER_CMD_BASE + 2)

// 状态机管理结构体
efsm_manage_t myStateMachine;

// 状态初始化函数
void initStateRunning(efsm_state_t *obj)
{
    (void)obj; // 防止未使用的参数警告
    printf("Initializing State: Running\n");
}

void initStateStopped(efsm_state_t *obj)
{
    (void)obj; // 防止未使用的参数警告
    printf("Initializing State: Stopped\n");
}

// 状态退出函数
void exitStateRunning(efsm_state_t *obj)
{
    (void)obj; // 防止未使用的参数警告
    printf("Exiting State: Running\n");
}

void exitStateStopped(efsm_state_t *obj)
{
    (void)obj; // 防止未使用的参数警告
    printf("Exiting State: Stopped\n");
}
// 状态事件处理函数
void actionStateRunning(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param);
void actionStateStopped(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param);
// 状态定义
static const struct efsm_state_ops ops = {
    .init   = initStateRunning,
    .exit   = exitStateRunning,
    .action = actionStateRunning,
};
static dzf_state_c2h2_lock_line_t state = {
    .node = {.ops = &ops},
};
efsm_state_t stateIdle = {
    .ops = &ops,
};
efsm_state_t stateRunning = {
    .ops = &ops,
};
efsm_state_t stateStopped = {
    .ops = &ops,
};
// 状态事件处理函数
void actionStateRunning(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param)
{
    (void)obj; // 防止未使用的参数警告
    (void)cmd;
    (void)param;
    if (cmd == CMD_STOP)
    {
        printf("Received CMD_STOP, transitioning to Stopped state\n");
        efsm_transition(obj->parent, &stateStopped);
    }
}

void actionStateStopped(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param)
{
    (void)obj; // 防止未使用的参数警告
    (void)cmd;
    (void)param;
    if (cmd == CMD_START)
    {
        printf("Received CMD_START, transitioning to Running state\n");
        efsm_transition(obj->parent, &stateRunning);
    }
}

// 主程序
int main()
{
    printf("hello world\n");
    // 初始化状态机框架
    efsm_init();

    // 初始化状态机管理结构体
    efsm_manage_init(&myStateMachine);

    // 注册状态机
    efsm_register(&myStateMachine);

    // 设置状态机管理函数


    // 初始化状态机状态为Running
    efsm_transition(&myStateMachine, &stateRunning);

    // 模拟状态机周期性执行
    for (int i = 0; i < 5; ++i)
    {
        printf("Executing State Machine Tick #%d\n", i + 1);
        efsm_manage_tick();
    }

    // 发送停止命令
    efsm_event_process(&myStateMachine, CMD_STOP, NULL);

    // 模拟状态机周期性执行
    for (int i = 0; i < 3; ++i)
    {
        printf("Executing State Machine Tick #%d\n", i + 6);
        efsm_manage_tick();
    }

    // 发送启动命令
    efsm_event_process(&myStateMachine, CMD_START, NULL);

    // 模拟状态机周期性执行
    for (int i = 0; i < 4; ++i)
    {
        printf("Executing State Machine Tick #%d\n", i + 9);
        efsm_manage_tick();
    }

    return 0;
}
