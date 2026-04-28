#ifndef GTHR_PLATFORM_H
#define GTHR_PLATFORM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

void gt_platform_memzero(void *ptr, size_t size);
void gt_platform_memcpy(void *dst, const void *src, size_t size);
int gt_platform_strcmp(const char *lhs, const char *rhs);
uint64_t gt_platform_now_us(void);
void gt_platform_print(const char *fmt, ...);
void gt_platform_install_alarm(void (*handler)(int));
void gt_platform_install_sigint(void (*handler)(int));
void gt_platform_rearm_alarm(void);
void gt_platform_sleep_ns(time_t sec, long nanosec);
void gt_platform_exit(int code) __attribute__((noreturn));

#endif