#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <calcexpr/calculator.h>

#include "unity.h"

#define EPSILON 1e-6

typedef struct {
    const char *name;
    const char *expression;
    double expected;
} success_case_t;

typedef struct {
    const char *name;
    const char *expression;
    calc_error_t expected_error;
    int exact_match;
} error_case_t;

static const success_case_t g_success_cases[] = {
    {"加法", "1+2", 3.0},
    {"减法", "10-3", 7.0},
    {"乘法", "2*3", 6.0},
    {"除法", "7/2", 3.5},
    {"优先级1", "2+3*4", 14.0},
    {"优先级2", "(2+3)*4", 20.0},
    {"负数1", "-5+3", -2.0},
    {"负数2", "(-1)*(-1)", 1.0},
    {"小数1", "0.5+0.25", 0.75},
    {"小数2", ".5 + .5", 1.0},
    {"科学计数法", "1e2+3", 103.0},
    {"微小非零除数", "1/0.0000000000001", 10000000000000.0},
    {"链式", "100/2/5", 10.0},
    {"复杂表达式", "((1+2)*(3-1))+5", 11.0},
    {"空格容忍", "  3 + 4 * 2  ", 11.0},
};

static const error_case_t g_error_cases[] = {
    {"空串", "", CALC_ERROR_NULL_EXPR, 1},
    {"全空白", "   ", CALC_ERROR_UNEXPECTED_TOKEN, 1},
    {"直接除零", "1/0", CALC_ERROR_DIV_BY_ZERO, 1},
    {"间接除零", "1/(2-2)", CALC_ERROR_DIV_BY_ZERO, 1},
    {"非法字符", "1+a", CALC_ERROR_INVALID_CHAR, 1},
    {"双小数点", "1..2", CALC_ERROR_INVALID_CHAR, 0},
    {"缺少右括号", "(1+2", CALC_ERROR_MISSING_RPAREN, 1},
    {"多余右括号", "(1+2))", CALC_ERROR_SYNTAX, 1},
    {"尾部运算符", "1+2+", CALC_ERROR_UNEXPECTED_TOKEN, 1},
    {"孤立右括号", ")", CALC_ERROR_UNEXPECTED_TOKEN, 1},
    {"多余数字", "1 2", CALC_ERROR_SYNTAX, 1},
};

static int almost_equal(double lhs, double rhs) {
    const double diff = fabs(lhs - rhs);
    const double scale = fmax(1.0, fmax(fabs(lhs), fabs(rhs)));
    return diff <= EPSILON * scale;
}

void setUp(void) {}

void tearDown(void) {}

static void test_success_cases(void) {
    const size_t count = sizeof(g_success_cases) / sizeof(g_success_cases[0]);

    for (size_t i = 0; i < count; ++i) {
        const success_case_t *tc = &g_success_cases[i];
        double result = 0.0;
        size_t err_pos = 0;
        const calc_error_t err = evaluate(tc->expression, &result, &err_pos);

        TEST_ASSERT_EQUAL_INT_MESSAGE(CALC_OK, err, tc->name);
        TEST_ASSERT_TRUE_MESSAGE(almost_equal(result, tc->expected), tc->name);
    }
}

static void test_error_cases(void) {
    const size_t count = sizeof(g_error_cases) / sizeof(g_error_cases[0]);

    for (size_t i = 0; i < count; ++i) {
        const error_case_t *tc = &g_error_cases[i];
        double result = 0.0;
        size_t err_pos = 0;
        const calc_error_t err = evaluate(tc->expression, &result, &err_pos);

        if (tc->exact_match) {
            TEST_ASSERT_EQUAL_INT_MESSAGE(tc->expected_error, err, tc->name);
        } else {
            TEST_ASSERT_TRUE_MESSAGE(err != CALC_OK, tc->name);
        }
    }
}

static void test_result_null_returns_null_expr(void) {
    size_t err_pos = 0;

    TEST_ASSERT_EQUAL_INT(CALC_ERROR_NULL_EXPR, evaluate("1+2", NULL, &err_pos));
}

static void test_expression_null_returns_null_expr(void) {
    size_t err_pos = 0;

    TEST_ASSERT_EQUAL_INT(CALC_ERROR_NULL_EXPR, evaluate(NULL, NULL, &err_pos));
}

static void test_recursion_limit(void) {
    const size_t depth = 320;
    const size_t expr_len = depth * 2 + 1;
    char *expr = (char *)malloc(expr_len + 1);
    double value = 0.0;
    size_t err_pos = 0;

    TEST_ASSERT_NOT_NULL(expr);
    memset(expr, '(', depth);
    expr[depth] = '1';
    memset(expr + depth + 1, ')', depth);
    expr[expr_len] = '\0';

    TEST_ASSERT_EQUAL_INT(CALC_ERROR_RECURSION_LIMIT, evaluate(expr, &value, &err_pos));

    free(expr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_success_cases);
    RUN_TEST(test_error_cases);
    RUN_TEST(test_result_null_returns_null_expr);
    RUN_TEST(test_expression_null_returns_null_expr);
    RUN_TEST(test_recursion_limit);
    return UNITY_END();
}
