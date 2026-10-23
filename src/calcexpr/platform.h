/**
 * @file platform.h
 * @brief 平台适配层头文件
 *
 * 提供跨平台的统一接口,隐藏不同平台之间的差异
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include <calcexpr/compiler.h>

/* ========================================================================
 * 跨平台类型定义
 * ======================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ========================================================================
 * 跨平台函数声明
 * ======================================================================== */

/**
 * @brief 平台初始化
 * @return 0 表示成功,非0表示失败
 */
int platform_init(void);

/**
 * @brief 平台清理
 */
void platform_cleanup(void);

/**
 * @brief 获取平台名称
 * @return 平台名称字符串
 */
const char *platform_get_name(void);

/**
 * @brief 获取当前时间戳(毫秒)
 * @return 时间戳
 */
uint64_t platform_get_tick_ms(void);

/**
 * @brief 睡眠指定毫秒数
 * @param ms 毫秒数
 */
void platform_sleep_ms(uint32_t ms);

/**
 * @brief 启用 UTF-8 控制台输出
 *
 * 确保控制台正确输出 UTF-8 编码的中文字符
 */
void platform_enable_utf8(void);

#endif /* PLATFORM_H */
