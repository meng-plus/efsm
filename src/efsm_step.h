/**
 * @file efsm_step.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 状态机步骤
 * @version 0.1
 * @date 2025-02-25
 *
 * @copyright Copyright (c) 2025  Gltech
 *
 * **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2025-02-25 <td>1.0     <td>mengplus     <td>create
 * </table>
 */
#ifndef __EFSM_STEP_H_
#define __EFSM_STEP_H_

#include <stdint.h>
#include <stdbool.h>
#include "efsm.h"

typedef enum
{
    EFSM_STEP_BASE,        /**< 基础步骤 */
    EFSM_STEP_TIMED,       /**< 带时间限制的步骤 */
    EFSM_STEP_CONDITIONAL, /**< 带条件判断的步骤 */
    EFSM_STEP_WARNING,     /**< 带条件判断的步骤 */
    EFSM_STEP_CUSTOM       /**< 自定义步骤 */
} efsm_step_type_t;
typedef struct efsm_state_step efsm_state_step_t;
/**
 * @brief EFSM 步骤基类结构体
 *
 * 所有步骤均继承自此结构体。
 */
typedef struct efsm_step
{
    efsm_step_type_t type; /**< 步骤类型标识 */
    const char *name;      /*!< 步骤名称 */
    /**
     * @brief 执行步骤的动作 执行一次
     *
     * @param[in] self 指向当前状态的指针
     */
    bool (*set)(efsm_state_step_t *self);
    /**
     * @brief 检查动作
     * @return true 动作执行完毕
     * @return false 动作未执行完毕
     */
    bool (*check)(efsm_state_step_t *self);
} efsm_step_t;

typedef const struct efsm_step *const_efsm_step_t;

/**
 * @brief 带时间限制的 EFSM 步骤结构体
 *
 * 在基类基础上增加了时间限制。
 */
typedef struct efsm_step_timed
{
    struct efsm_step base;   /**< 基类结构体 */
    uint32_t limit_min_time; /*!< ms 限制步骤最少执行时间 0标识不设限 */
    uint32_t limit_max_time; /*!< ms 限制超时时间 0标识不设限 */
} *efsm_step_timed_t;
/**
 * @brief 带条件判断的 EFSM 步骤结构体
 *
 * 在基类基础上增加了条件判断功能。
 */
typedef struct efsm_step_conditional
{
    struct efsm_step base; /**< 基类结构体 */
    int16_t step_true;     /*!< 条件成立 步骤序号 */
    int16_t step_false;    /*!< 条件不成立 */
} *efsm_step_conditional_t;

/**
 * @brief 带异常处理的 EFSM 步骤结构体
 *
 * 在基类基础上增加了异常状态。
 */
typedef struct efsm_step_warning
{
    struct efsm_step base; /**< 基类结构体 */
    uint16_t step_id;      /*!< 报错ID */
} *efsm_step_warning_t;

#define EFSM_STEP_DEFAULT(n_name, n_type, n_set, n_check) \
    {                                                     \
        .name  = n_name,                                  \
        .type  = n_type,                                  \
        .set   = n_set,                                   \
        .check = n_check,                                 \
    }
#define EFSM_STEP_TIMED_DEFAULT(n_name, n_set, n_check, min_time, max_time)           \
    {                                                                                 \
        .base           = EFSM_STEP_DEFAULT(n_name, EFSM_STEP_TIMED, n_set, n_check), \
        .limit_min_time = min_time,                                                   \
        .limit_max_time = max_time,                                                   \
    }
#define EFSM_STEP_CONDITIONAL_DEFAULT(n_name, n_set, n_check, n_step_true, n_step_false) \
    {                                                                                    \
        .base       = EFSM_STEP_DEFAULT(n_name, EFSM_STEP_CONDITIONAL, n_set, n_check),  \
        .step_true  = n_step_true,                                                       \
        .step_false = n_step_false,                                                      \
    }

struct efsm_state_step
{
    efsm_state_t state;
    uint16_t step;
    uint16_t step_num;
    const_efsm_step_t *step_vec;
};

/**
 * @brief 状态机步骤执行
 *
 * @param state_ptr 状态指针
 * @param pstep 步骤列表
 * @param step_num 步骤数量
 * @param tick 当前时间
 * @return true  成功执行
 * @return false 异常执行/无步骤可执行
 */
bool efsm_step_process(efsm_state_step_t *state_step_ptr, uint32_t tick);
bool efsm_step_set(efsm_state_step_t *state_step_ptr, uint8_t step, uint32_t tick);
void efsm_step_init(efsm_state_step_t *state_step_ptr, const_efsm_step_t *step_vec, uint16_t step_num, uint32_t tick);
#endif
