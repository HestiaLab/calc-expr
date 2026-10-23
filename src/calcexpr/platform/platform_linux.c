/**
 * @file platform_linux.c
 * @brief Linux 平台特定实现
 */

/*
 * 定义 POSIX 特性宏以启用 clock_gettime 和 nanosleep
 * 必须在所有 include 之前定义
 */
#define _POSIX_C_SOURCE 199309L // NOLINT(bugprone-reserved-identifier)

#include "platform.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <time.h>

/* ========================================================================
 * 公共函数实现
 * ======================================================================== */

int platform_init(void) {
    /* Linux 平台初始化 - 当前无需特殊处理 */
    return 0;
}

void platform_cleanup(void) {
    /* Linux 平台清理 - 当前无需特殊处理 */
}

const char *platform_get_name(void) {
    return "Linux";
}

uint64_t platform_get_tick_ms(void) {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* 转换为毫秒 */
    return (uint64_t)ts.tv_sec * 1000U + (uint64_t)ts.tv_nsec / 1000000U;
}

void platform_sleep_ms(uint32_t ms) {
    struct timespec request;
    request.tv_sec = (time_t)(ms / 1000U);
    request.tv_nsec = (long)(ms % 1000U) * 1000000L;

    while (nanosleep(&request, &request) == -1 && errno == EINTR) {
        /* 被信号打断时继续剩余时间 */
    }
}

void platform_enable_utf8(void) {
    // 设置本地化以支持 UTF-8
    setlocale(LC_ALL, "");
}
