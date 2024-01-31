/**
 * @file efsm_def.h
 * @brief 事件驱动型状态机内部支持的命令定义
 * @author GL2715 (chengmeng_2@outlook.com)
 * @version  0.1
 * @date 2024-01-31
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __EFSM_DEF_H_
#define __EFSM_DEF_H_

/**
 * @brief 状态事件命令段划分
 * [0x00-0xFF]为框架使用字段,不得用于用户命令
 *
 */
#define EFSM_STATE_SYS_CMD_BASE 0x000
#define EFSM_STATE_USER_CMD_BASE 0x100
#define EFSM_UNUSED(param) (void)param
// 根据需要定义更多的命令
enum EFSM_SYS_CMD
{
    EFSM_CMD_STOP = EFSM_STATE_SYS_CMD_BASE,
    EFSM_CMD_START,
    EFSM_CMD_HOLD, /*!< param  uint32_t*  {0,1}  */
    EFSM_CMD_END,

};

#endif
