#include "efsm_step.h"

bool efsm_step_process(efsm_state_step_t *state_step_ptr, uint32_t tick)
{
    efsm_state_t *state_ptr     = &state_step_ptr->state;
    const_efsm_step_t *step_vec = state_step_ptr->step_vec;
    uint16_t step_num           = state_step_ptr->step_num;

    if (!state_ptr || !step_vec || step_num == 0 || state_step_ptr->step >= step_num)
    {
        return false;
    }
    const uint8_t idx          = state_step_ptr->step;
    const_efsm_step_t step_ptr = step_vec[idx];
    if (!step_ptr)
    {
        return false;
    }

    bool res = 0;
    if (step_ptr->check) res = step_ptr->check(state_step_ptr);

    switch (step_ptr->type)
    {
    case EFSM_STEP_BASE:
        if (res) state_step_ptr->step++;
        break;
    case EFSM_STEP_TIMED: {
        efsm_step_timed_t step_time    = EFSM_GET_STRUCT_PTR(step_ptr, struct efsm_step_timed, base);
        uint32_t          elapsed_time = (uint32_t)(tick - state_ptr->timestamp);

        if (res)
        {
            // 条件满足，检查最小时间限制
            if (!step_time->limit_min_time || elapsed_time >= step_time->limit_min_time)
            {
                state_step_ptr->step++;
            }
        }
        else
        {
            // 条件不满足，检查最大时间限制
            if (step_time->limit_max_time && elapsed_time >= step_time->limit_max_time)
            {
                state_step_ptr->step++;
            }
        }
    }
    break;
    case EFSM_STEP_CONDITIONAL: {
        efsm_step_conditional_t step_conditional = EFSM_GET_STRUCT_PTR(step_ptr, struct efsm_step_conditional, base);
        if (res)
        {
            state_step_ptr->step += step_conditional->step_true;
        }
        else
        {
            state_step_ptr->step += step_conditional->step_false;
        }
    }
    break;
    default:
        return false;
        break;
    }
    if (state_step_ptr->step != idx)
    {
        efsm_step_set(state_step_ptr, state_step_ptr->step, tick);
    }
    return true;
}

bool efsm_step_set(efsm_state_step_t *state_step_ptr, uint8_t step, uint32_t tick)
{
    if (!state_step_ptr || !state_step_ptr->step_vec || step >= state_step_ptr->step_num)
    {
        return false;
    }

    state_step_ptr->step = step;
    if (state_step_ptr->step_vec[step] && state_step_ptr->step_vec[step]->set)
    {
        state_step_ptr->step_vec[step]->set(state_step_ptr);
    }
    state_step_ptr->state.timestamp = tick;
    return true; // 返回操作成功
}
