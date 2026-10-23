/**
 * @file calculator.h
 * @brief 计算器公共接口定义
 */

#ifndef CALCEXPR_CALCULATOR_H
#define CALCEXPR_CALCULATOR_H

#include <stddef.h>

#include <calcexpr/export.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * 错误码枚举
 * ======================================================================== */

typedef enum {
    CALC_OK = 0,                 /**< 成功，无错误 */
    CALC_ERROR_NULL_EXPR,        /**< 表达式为空或 NULL */
    CALC_ERROR_INVALID_CHAR,     /**< 包含非法字符 */
    CALC_ERROR_DIV_BY_ZERO,      /**< 除数为零 */
    CALC_ERROR_UNEXPECTED_TOKEN, /**< 意外的 token */
    CALC_ERROR_MISSING_RPAREN,   /**< 缺少右括号 ) */
    CALC_ERROR_SYNTAX,           /**< 语法错误 */
    CALC_ERROR_RECURSION_LIMIT,  /**< 递归深度超限 */
    CALC_ERROR_NUMBER_OVERFLOW,  /**< 数值超出范围 */
    CALC_ERROR_LAST              /**< 错误码边界标记 */
} calc_error_t;

/* ========================================================================
 * 公共函数声明
 * ======================================================================== */

/**
 * @brief 评估数学表达式
 *
 * 解析并计算表达式的值。
 *
 * @param expression 要计算的数学表达式字符串
 * @param result    计算结果输出
 * @param err_pos   错误位置输出（可为 NULL）
 * @return 错误码，CALC_OK 表示成功
 */
CALCEXPR_API calc_error_t evaluate(const char *expression, double *result, size_t *err_pos);

/**
 * @brief 获取错误消息
 *
 * @param err 错误码
 * @return 错误码对应的字符串描述
 */
CALCEXPR_API const char *calc_get_error_message(calc_error_t err);

/**
 * @brief 获取解析器错误消息（别名）
 *
 * @param err 错误码
 * @return 错误码对应的字符串描述
 */
CALCEXPR_API const char *parser_get_error_message(calc_error_t err);

#ifdef __cplusplus
}
#endif

#endif /* CALCEXPR_CALCULATOR_H */
