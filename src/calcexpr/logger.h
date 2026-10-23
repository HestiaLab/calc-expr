#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

#include <calcexpr/export.h>

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } log_level_t;

/**
 * 初始化日志系统，设置最小输出级别
 */
CALCEXPR_API void logger_init(log_level_t level);

/**
 * 动态调整日志级别
 */
CALCEXPR_API void logger_set_level(log_level_t level);

/**
 * 获取当前日志级别（便于调试/测试）
 */
CALCEXPR_API log_level_t logger_get_level(void);

/**
 * 输出日志。WARNING/ERROR 默认写 stderr，其余写 stdout。
 * 调用方无需手动换行，如果 format 已包含换行则按原样输出。
 */
CALCEXPR_API void logger_log(log_level_t level, const char *format, ...);

#endif /* LOGGER_H */
