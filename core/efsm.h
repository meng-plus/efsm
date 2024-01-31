/**
 * @file efsm.h
 * @brief 基于事件型的状态机
 * @author GL2715 (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __EFSM_H_
#define __EFSM_H_

#include <stdint.h>
#include "efsm_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _EFSM_STATE
    {
        struct _EFSM_STATE *next;                             /*!< 要切换的下一个状态，如果为NULL则不切换 */
        void (*init)(void *obj);                              /*!< 切换到这个状态的初始化操作 */
        void (*exit)(void *obj);                              /*!< 退出此状态的动作 */
        void (*action)(void *obj, uint32_t cmd, void *param); /*!< 相应的状态事件 */
    } efsm_state_t;

    typedef struct _EFSM_MANAGE
    {
        uint32_t hold_on : 1;      /*!< 锁定状态不允许切换 */
        uint32_t stop : 1;         /*!< 停止事件响应 */
        struct _EFSM_MANAGE *next; /*!< 单链表结构 */
        efsm_state_t *pstate;
        void (*init)(void *obj);                               /* 初始化函数 */
        void (*tick)(void *obj);                               /* 周期性任务 */
        void (*exit)(void *obj);                               /* 退出函数 */
        void (*control)(void *obj, uint32_t cmd, void *param); /* 控制函数 ,param取决于cmd*/
    } efsm_manage_t;

    void efsm_init();
    /**
     * @brief 初始化结构体，用于清零缓存区，可以自行处理
     */
    void efsm_manage_init(efsm_manage_t *obj);
    /**
     * @brief 注册状态机，使其可用
     */
    void efsm_register(efsm_manage_t *obj);
    /**
     * @brief 移除状态机
     * @param obj 移除的状态机句柄
     */
    void efsm_remove(efsm_manage_t *obj);
    /**
     * @brief 周期性事件函数，可以理解为状态机的线程，根据需要开发
     */
    void efsm_manage_tick();
    // 执行状态事件处理
    void efsm_process_event(efsm_manage_t *obj, uint32_t cmd, void *param);
    void efsm_broadcast_event(uint32_t cmd, void *param);
    // 执行状态切换
    void efsm_transition(efsm_manage_t *obj, efsm_state_t *nextState);
#ifdef __cplusplus
}
#endif

#endif
