#include <stdbool.h>

#include "command.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

static void test_command_state_init_defaults(void) {
    command_state_t state;

    command_state_init(&state);

    TEST_ASSERT_FALSE(state.show_process);
    TEST_ASSERT_FALSE(state.should_exit);
    TEST_ASSERT_EQUAL_INT(INPUT_MODE_NORMAL, state.interactive.mode);
}

static void test_command_dispatch_flow(void) {
    command_state_t state;
    command_result_t result;

    command_state_init(&state);

    result = command_dispatch("show process", &state);
    TEST_ASSERT_EQUAL_INT(COMMAND_RESULT_HANDLED, result);
    TEST_ASSERT_EQUAL_INT(INPUT_MODE_DEBUG_LEVEL, state.interactive.mode);

    command_handle_interactive("4", &state);
    TEST_ASSERT_TRUE(state.show_process);
    TEST_ASSERT_EQUAL_INT(INPUT_MODE_NORMAL, state.interactive.mode);

    result = command_dispatch("hide process", &state);
    TEST_ASSERT_EQUAL_INT(COMMAND_RESULT_HANDLED, result);
    TEST_ASSERT_FALSE(state.show_process);

    result = command_dispatch("show help", &state);
    TEST_ASSERT_EQUAL_INT(COMMAND_RESULT_HANDLED, result);

    result = command_dispatch("show unknown", &state);
    TEST_ASSERT_EQUAL_INT(COMMAND_RESULT_ERROR, result);

    result = command_dispatch("2+3", &state);
    TEST_ASSERT_EQUAL_INT(COMMAND_RESULT_NOT_COMMAND, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_command_state_init_defaults);
    RUN_TEST(test_command_dispatch_flow);
    return UNITY_END();
}
