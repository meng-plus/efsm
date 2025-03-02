/**
 * @file efsm.c
 * @brief
 * @author mengplus (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2024  Zhengzhou GL. TECH Co.,Ltd
 *
 */
#include "efsm.h"

#include <string.h>

/**
 * @brief 事件框架句柄
 * @author mengplus (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 *
 */
static efsm_manage_t *efsm_list_head        = NULL;
static efsm_transition_hook transition_hook = NULL;
// 初始化状态机框架
void efsm_init()
{
    // efsm_list_head = NULL;
}

// 初始化状态机管理结构体
void efsm_manage_init(efsm_manage_t *obj)
{
    memset(obj, 0, sizeof(efsm_manage_t));
    obj->init_ok = 1;
}

// 注册状态机
void efsm_register(efsm_manage_t *obj)
{
    efsm_manage_t *current = efsm_list_head;
    while (current != NULL)
    { /*!< 避免重复注册 */
        if (current == obj)
        {
            return;
        }
        current = current->next;
    }
    if (obj && obj->init_ok)
    {
        obj->next      = efsm_list_head;
        efsm_list_head = obj;

        if (obj->ops && obj->ops->init)
        {
            obj->ops->init(obj);
        }
    }
}

// 从状态机链表中移除状态机
void efsm_remove(efsm_manage_t *obj)
{
    efsm_manage_t *current = efsm_list_head;
    efsm_manage_t *prev    = NULL;

    while (current)
    {
        if (current == obj)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                efsm_list_head = current->next;
            }
            if (obj->ops && obj->ops->exit)
            {
                obj->ops->exit(obj);
            }
            return;
        }
        prev    = current;
        current = current->next;
    }
}

// 执行所有注册的状态机的周期性任务
void efsm_manage_tick()
{
    efsm_manage_t *current = efsm_list_head;

    while (current)
    {
        if (current->ops && !current->stop)
        {
            current->ops->tick(current);
        }
        current = current->next;
    }
}

void efsm_manage_tick_user(efsm_manage_t *obj)
{
    if (obj)
    {
        if (obj->ops != NULL && !obj->stop)
        {
            obj->ops->tick(obj);
        }
    }
}

void efsm_manage_control(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param)
{
    if (obj && obj->ops && obj->ops->control)
    {
        obj->ops->control(obj, cmd, param);
    }
}

// 执行状态切换
void efsm_transition(efsm_manage_t *obj, efsm_state_t *nextState)
{
    if (transition_hook) transition_hook(obj, nextState);

    if ((obj->pstate != nextState) && !obj->hold_on)
    {
        if (obj->pstate)
        {
            if (obj->pstate->ops && obj->pstate->ops->exit)
            {
                obj->pstate->ops->exit(obj->pstate);
            }
            obj->pstate->parent = NULL;
            obj->pstate         = NULL;
        }
        if (nextState)
        {
            nextState->parent = obj;
            obj->pstate       = nextState;
            if (nextState->ops && nextState->ops->init)
            {
                nextState->ops->init(obj->pstate);
            }
        }
    }
}

void efsm_transition_set_hook(efsm_transition_hook hook)
{
    transition_hook = hook;
}

// 执行状态事件处理
void efsm_event_process(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param)
{
    if ((cmd & ~0xFF) == 0)
    { /*!< 系统级别的事件 */
        efsm_event_sys(obj, cmd, param);
    }
    else if (obj)
    { /*!< 用户级别的事件 */

        if (obj->pstate && obj->pstate->ops && obj->pstate->ops->action)
        {
            obj->pstate->ops->action(obj->pstate, cmd, param);
        }
    }
}

efsm_state_t *efsm_manage_get_state(efsm_manage_t *obj)
{
    if (obj == NULL)
    {
        return NULL;
    }
    return obj->pstate;
}

void *efsm_manage_get_userdata(efsm_manage_t *obj)
{
    if (obj)
    {
        return obj->user_data;
    }
    return NULL;
}

void efsm_event_broadcast(uint32_t cmd, efsm_param_t *param)
{
    efsm_manage_t *current = efsm_list_head;

    while (current)
    {
        efsm_event_process(current, cmd, param);
        current = current->next;
    }
}
// 你可以根据需要定义更多的状态和事件处理函数

void efsm_event_sys(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param)
{
    if (obj)
    {
        return;
    }
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
