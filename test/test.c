#include "efsm.h"
#include <stdbool.h>
#include <stdio.h>
#include <check.h>
#include <stdlib.h>
// 模拟状态 1
static void state1_init(efsm_state_t *obj)
{
    printf("State1: Init\n");
}

static void state1_exit(efsm_state_t *obj)
{
    printf("State1: Exit\n");
}

static void state1_action(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param)
{
    printf("State1: Action - CMD: %u\n", cmd);
}

static const efsm_state_ops_t state1_ops = {
    .name   = "State1",
    .init   = state1_init,
    .exit   = state1_exit,
    .action = state1_action,
};

efsm_state_t state1 = {
    .ops = &state1_ops,
};

// 模拟状态 2
static void state2_init(efsm_state_t *obj)
{
    printf("State2: Init\n");
}

static void state2_exit(efsm_state_t *obj)
{
    printf("State2: Exit\n");
}

static void state2_action(efsm_state_t *obj, uint32_t cmd, efsm_param_t *param)
{
    printf("State2: Action - CMD: %u\n", cmd);
}

static const efsm_state_ops_t state2_ops = {
    .name   = "State2",
    .init   = state2_init,
    .exit   = state2_exit,
    .action = state2_action,
};

efsm_state_t state2 = {
    .ops = &state2_ops,
};
// 测试状态机初始化
START_TEST(test_efsm_init)
{
    efsm_manage_t state_machine;
    efsm_manage_init(&state_machine);
    ck_assert_int_eq(state_machine.init_ok, 1);
}
END_TEST

// 测试状态切换
START_TEST(test_efsm_transition)
{
    efsm_manage_t state_machine;
    efsm_manage_init(&state_machine);
    efsm_register(&state_machine);

    // 切换到 State1
    efsm_transition(&state_machine, &state1);
    ck_assert_ptr_eq(state_machine.pstate, &state1); // 当前状态应为 State1

    // 切换到 State2
    efsm_transition(&state_machine, &state2);
    ck_assert_ptr_eq(state_machine.pstate, &state2); // 当前状态应为 State2
}
END_TEST

// 测试事件处理
START_TEST(test_efsm_event_process)
{
    efsm_manage_t state_machine;
    efsm_manage_init(&state_machine);
    efsm_register(&state_machine);

    // 切换到 State1
    efsm_transition(&state_machine, &state1);

    // 处理事件
    efsm_param_t param = {EFSM_PARAM_NULL, 0, 0};
    efsm_event_process(&state_machine, 1, &param); // 应调用 State1 的 action 函数
}
END_TEST
Suite *efsm_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("EFSM");

    // 核心测试用例
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_efsm_init);
    tcase_add_test(tc_core, test_efsm_transition);
    tcase_add_test(tc_core, test_efsm_event_process);
    suite_add_tcase(s, tc_core);

    return s;
}
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s  = efsm_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
