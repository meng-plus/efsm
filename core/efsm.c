/**
 * @file efsm.c
 * @brief
 * @author GL2715 (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2024  Zhengzhou GL. TECH Co.,Ltd
 *
 */
#include "efsm.h"

#include <string.h>

// 实现状态初始化
void efsm_state_init(efsm_manage_t *obj);
// 实现状态退出
void efsm_state_exit(efsm_manage_t *obj);
// 系统级别的事件处理
void efsm_sys_event(efsm_manage_t *obj, uint32_t cmd, void *param);
/**
 * @brief 事件框架句柄
 * @author GL2715 (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 *
 */
static efsm_manage_t *efsm_list_head = NULL;

// 初始化状态机框架
void efsm_init()
{
    efsm_list_head = NULL;
}

// 初始化状态机管理结构体
void efsm_manage_init(efsm_manage_t *obj)
{
    memset(obj, 0, sizeof(efsm_manage_t));
}

// 注册状态机
void efsm_register(efsm_manage_t *obj)
{
    obj->next = efsm_list_head;
    efsm_list_head = obj;
    efsm_state_init(obj);
}

// 从状态机链表中移除状态机
void efsm_remove(efsm_manage_t *obj)
{
    efsm_manage_t *current = efsm_list_head;
    efsm_manage_t *prev = NULL;

    while (current != NULL)
    {
        if (current == obj)
        {
            if (prev == NULL)
            {
                efsm_list_head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            efsm_state_exit(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 执行所有注册的状态机的周期性任务
void efsm_manage_tick()
{
    efsm_manage_t *current = efsm_list_head;

    while (current != NULL)
    {
        if (current->tick != NULL && !current->stop)
        {
            current->tick(current);
        }
        current = current->next;
    }
}

// 执行状态切换
void efsm_transition(efsm_manage_t *obj, efsm_state_t *nextState)
{
    if ((obj->pstate != nextState) &&
        !obj->hold_on)
    {
        if (obj->pstate != NULL && obj->pstate->exit != NULL)
        {
            obj->pstate->exit(obj->pstate);
        }
        obj->pstate = nextState;
        if (obj->pstate != NULL && obj->pstate->init != NULL)
        {
            obj->pstate->init(obj->pstate);
        }
    }
}

// 执行状态事件处理
void efsm_process_event(efsm_manage_t *obj, uint32_t cmd, void *param)
{
    if (obj->pstate != NULL &&
        !obj->stop &&
        obj->pstate->action != NULL)
    {
        if ((cmd & ~0xFF) == 0)
        { /*!< 系统级别的事件 */
            efsm_sys_event(obj, cmd, param);
        }
        else
        { /*!< 用户级别的事件 */
            obj->pstate->action(obj->pstate, cmd, param);
        }
        if (obj->pstate->next)
        {
            efsm_transition(obj, obj->pstate->next);
        }
    }
}

void efsm_broadcast_event(uint32_t cmd, void *param)
{
    efsm_manage_t *current = efsm_list_head;

    while (current != NULL)
    {
        if (!current->stop)
        {
            efsm_process_event(current, cmd, param);
        }
        current = current->next;
    }
}
// 你可以根据需要定义更多的状态和事件处理函数

void efsm_sys_event(efsm_manage_t *obj, uint32_t cmd, void *param)
{
    switch (cmd)
    {
    case EFSM_CMD_STOP: // 切换到空状态表示停止状态机
        obj->stop = 1;
        break;
    case EFSM_CMD_START: // 继续当前状态
        obj->stop = 0;
        break;
    case EFSM_CMD_HOLD:
        obj->hold_on = *(uint32_t *)param != 0;
        break;
    case EFSM_CMD_END:
        break;
    default:
        break;
    }
}

// 实现状态初始化
void efsm_state_init(efsm_manage_t *obj)
{
    if (obj->init != NULL)
    {
        obj->init(obj);
    }
}

// 实现状态退出
void efsm_state_exit(efsm_manage_t *obj)
{
    if (obj->exit != NULL)
    {
        obj->exit(obj);
    }
}
