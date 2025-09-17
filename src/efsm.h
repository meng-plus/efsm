/**
 * @file efsm.h
 * @brief 基于事件型的状态机
 * @author mengplus (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __EFSM_H_
#define __EFSM_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> //for offsetof
#include "efsm_def.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum EFSM_PARAM_TYPE
{
    EFSM_PARAM_NULL,
    EFSM_PARAM_U8,
    EFSM_PARAM_U16,
    EFSM_PARAM_U32,
    EFSM_PARAM_F,
    EFSM_PARAM_PTR,
    EFSM_PARAM_U64,
    EFSM_PARAM_D,
    EFSM_PARAM_STRUCT,
    EFSM_PARAM_BASE,
} efsm_param_type_t;

/**
 * @brief EFSM传参的数据类型定义
 *
 */
typedef struct _EFSM_PARAM_BASE
{
    uint8_t type; /*!< 数据类型 @see  efsm_param_type_t */
    uint8_t size; /*!< 数据长度 */
    uint16_t len; /*!< 数据数量 */
} efsm_param_t;

// 默认初始化宏
#define EFSM_PARAM_DEFAULT(ntype, nsize, nlen) \
    {                                          \
        .type = ntype,                         \
        .size = nsize,                         \
        .len  = nlen,                          \
    }
typedef struct _EFSM_STATE efsm_state_t;
typedef struct _EFSM_MANAGE efsm_manage_t;
typedef struct efsm_state_ops
{
    const char *name;                    /*!< 状态名称 */
    void (*init)(efsm_state_t *obj);     /*!< 切换到这个状态的初始化操作 */
    void (*exit)(efsm_state_t *obj);     /*!< 退出此状态的动作 */
    void (*action)(efsm_state_t *obj, uint32_t cmd,
                   efsm_param_t *param); /*!< 相应的状态事件 */
} efsm_state_ops_t;

struct _EFSM_STATE
{
    efsm_manage_t *parent;       /*!< 状态句柄 非运行状态此处为空*/
    uint32_t timestamp;          /*!< 时间戳 */
    const efsm_state_ops_t *ops; /*!< 操作接口 */
};

/**
 * @brief 状态切换钩子用于检测状态切换
 *
 * @param obj
 * @param nextState
 */
typedef void (*efsm_transition_hook)(const efsm_manage_t *obj, const efsm_state_t *nextState);

typedef struct efsm_manage_ops
{
    const char *name;                     /*!< 状态机名称*/
    void (*init)(efsm_manage_t *obj);     /*!< 初始化函数 */
    void (*tick)(efsm_manage_t *obj);     /*!< 周期性任务 */
    void (*exit)(efsm_manage_t *obj);     /*!< 退出函数 */
    void (*control)(efsm_manage_t *obj, uint32_t cmd,
                    efsm_param_t *param); /*!< 控制函数 ,param取决于cmd*/
} efsm_manage_ops_t;
struct _EFSM_MANAGE
{
    uint32_t init_ok : 1;         /*!< 初始化标志 */
    uint32_t hold_on : 1;         /*!< 锁定状态不允许切换 */
    uint32_t stop    : 1;         /*!< 停止事件响应 */
    efsm_state_t *pstate;
    uint32_t timestamp;           /*!< 时间戳 上次切换状态标签 */
    const efsm_manage_ops_t *ops; /*!< 操作接口 */
    efsm_transition_hook hook;
    void *user_data;              /*!< 用户自定义数据 */
    efsm_manage_t *next;          /*!< 单链表结构 */
};
/** 类型转换 获取包含某成员的结构体指针
 * @example EFSM_GET_STRUCT_PTR(state_ptr,efsm_state_t,obj)
 */
#define EFSM_GET_STRUCT_PTR(member_ptr, struct_type, member_name) \
    ((struct_type *)((char *)(member_ptr) - offsetof(struct_type, member_name)))

void efsm_init(void);
/**
 * @brief 注册状态机，使其可用
 */
bool efsm_register(efsm_manage_t *obj);
/**
 * @brief 移除状态机
 * @param obj 移除的状态机句柄
 */
void efsm_remove(efsm_manage_t *obj);
/**
 * @brief 初始化结构体，用于清零缓存区，可以自行处理
 */
void efsm_manage_init(efsm_manage_t *obj);
/**
 * @brief 周期性事件函数，可以理解为状态机的线程，根据需要开发
 * 只有执行 efsm_register  才会从这里调用
 */
void efsm_manage_tick(void);
/**
 * @brief 用户自定义的线程函数
 * 在此处调用的efsm 不再需要注册
 *
 * @param obj efsm句柄
 */
void efsm_manage_tick_user(efsm_manage_t *obj);

/**
 * @brief 指定状态机的控制命令
 * 发给状态机管理
 * @param obj
 * @param cmd
 * @param param
 */
void efsm_manage_control(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param);
/**
 * @brief 获得当前状态
 *
 * @param obj
 * @return efsm_state_t*
 */
efsm_state_t *efsm_manage_get_state(efsm_manage_t *obj);
void *efsm_manage_get_userdata(efsm_manage_t *obj);

/**
 * @brief  执行状态事件处理
 * 只有状态(state)处理动作
 * @param obj
 * @param cmd @see enum EFSM_SYS_CMD
 * @param param
 */
void efsm_event_process(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param);
/**
 * @brief 管理状态机控制命令
 *
 * @param obj
 * @param cmd
 * @param param
 */
void efsm_event_sys(efsm_manage_t *obj, uint32_t cmd, efsm_param_t *param);
/**
 * @brief 广播命令
 *
 * @param cmd
 * @param param
 */
void efsm_event_broadcast(uint32_t cmd, efsm_param_t *param);
// 执行状态切换
void efsm_transition(efsm_manage_t *obj, efsm_state_t *nextState);

void efsm_transition_set_hook(efsm_manage_t *obj, efsm_transition_hook hook);
#ifdef __cplusplus
}
#endif

#endif
