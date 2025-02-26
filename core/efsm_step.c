#include "efsm_step.h"

bool efsm_step_process(efsm_state_t *state_ptr, const_efsm_step_t *step_vec, uint16_t step_num, uint32_t tick)
{
    if (state_ptr->step < step_num)
    {
        const_efsm_step_t step_ptr = step_vec[state_ptr->step];

        bool res = 0;
        if (step_ptr->action) res = step_ptr->action(state_ptr);

        switch (step_ptr->type)
        {
        case EFSM_STEP_BASE:
            if (res) state_ptr->step++;
            break;
        case EFSM_STEP_TIMED: {
            efsm_step_timed_t step_time = EFSM_GET_STRUCT_PTR(step_ptr, struct efsm_step_timed, base);
            if (res)
            {
                if (!step_time->limit_min_time || step_time->limit_min_time <= tick - state_ptr->timestamp)
                {
                    state_ptr->step++;
                }
            }
            else
            {
                if (step_time->limit_max_time && (step_time->limit_max_time >= tick - state_ptr->timestamp))
                {
                    state_ptr->step++;
                }
            }
        }
        break;
        case EFSM_STEP_CONDITIONAL: {
            efsm_step_conditional_t step_conditional = EFSM_GET_STRUCT_PTR(step_ptr, struct efsm_step_conditional, base);
            if (res)
            {
                state_ptr->step += step_conditional->step_true;
            }
            else
            {
                state_ptr->step += step_conditional->step_false;
            }
        }
        break;
        default:
            return false;
            break;
        }
        return true;
    }
    return false;
}
