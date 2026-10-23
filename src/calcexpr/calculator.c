/**
 * @file calculator.c
 * @brief 计算器公共接口实现
 */

#include <calcexpr/calculator.h>

#include "parser.h"

/* ========================================================================
 * 错误消息定义
 * ======================================================================== */

static const char *g_error_messages[] = {[CALC_OK] = "成功",
                                         [CALC_ERROR_NULL_EXPR] = "表达式为空",
                                         [CALC_ERROR_INVALID_CHAR] = "包含非法字符",
                                         [CALC_ERROR_DIV_BY_ZERO] = "除数为零",
                                         [CALC_ERROR_UNEXPECTED_TOKEN] = "意外的记号",
                                         [CALC_ERROR_MISSING_RPAREN] = "缺少右括号",
                                         [CALC_ERROR_SYNTAX] = "语法错误",
                                         [CALC_ERROR_RECURSION_LIMIT] = "表达式嵌套过深",
                                         [CALC_ERROR_NUMBER_OVERFLOW] = "数值超出可表示范围",
                                         [CALC_ERROR_LAST] = NULL};

/* ========================================================================
 * 编译期安全检查
 * ======================================================================== */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(g_error_messages) / sizeof(g_error_messages[0]) == CALC_ERROR_LAST + 1,
               "错误消息数组大小与 calc_error_t 枚举不一致");
#endif

/* ========================================================================
 * 公共 API 实现
 * ======================================================================== */

const char *calc_get_error_message(calc_error_t err) {
    const size_t count = sizeof(g_error_messages) / sizeof(g_error_messages[0]);

    if (err >= 0 && (size_t)err < count && g_error_messages[err] != NULL) {
        return g_error_messages[err];
    }

    return "未知错误";
}

const char *parser_get_error_message(calc_error_t err) {
    return calc_get_error_message(err);
}

/**
 * @brief 评估数学表达式
 *
 * 公共入口函数，对表达式进行验证后委托给 parser 模块。
 *
 * @param expression 要计算的数学表达式
 * @param result    结果输出指针
 * @param err_pos   错误位置（可为 NULL）
 * @return 错误码
 */
calc_error_t evaluate(const char *expression, double *result, size_t *err_pos) {
    /* 参数验证：表达式为空或结果指针为 NULL */
    if (expression == NULL || *expression == '\0' || result == NULL) {
        if (err_pos != NULL) {
            *err_pos = 0;
        }
        return CALC_ERROR_NULL_EXPR;
    }

    return parser_evaluate_expression(expression, result, err_pos);
}