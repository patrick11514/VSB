#include "gthr_platform.h"

#include "gthr.h"

#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void gt_platform_memzero(void *ptr, size_t size) { memset(ptr, 0, size); }

void gt_platform_memcpy(void *dst, const void *src, size_t size) {
  memcpy(dst, src, size);
}

int gt_platform_strcmp(const char *lhs, const char *rhs) {
  return strcmp(lhs, rhs);
}

uint64_t gt_platform_now_us(void) {
  struct timeval tv;

  if (gettimeofday(&tv, NULL) != 0)
    return 0;
  return (uint64_t)tv.tv_sec * 1000000ull + (uint64_t)tv.tv_usec;
}

void gt_platform_print(const char *fmt, ...) {
  va_list args;
  char buffer[1024];
  int length;

  va_start(args, fmt);
  length = vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  if (length < 0)
    return;
  if ((size_t)length > sizeof(buffer))
    length = (int)sizeof(buffer);
  (void)write(STDOUT_FILENO, buffer, (size_t)length);
}

void gt_platform_install_alarm(void (*handler)(int)) {
  struct sigaction action;

  memset(&action, 0, sizeof(action));
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGALRM, &action, NULL);
}

void gt_platform_install_sigint(void (*handler)(int)) {
  struct sigaction action;

  memset(&action, 0, sizeof(action));
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);
}

void gt_platform_rearm_alarm(void) {
  alarm(0);
  ualarm(500, 500);
}

void gt_platform_sleep_ns(time_t sec, long nanosec) {
  struct timespec req;

  req.tv_sec = sec;
  req.tv_nsec = nanosec;
  while (nanosleep(&req, &req) != 0) {
    if (errno != EINTR)
      break;
    gt_yield();
  }
}

void gt_platform_exit(int code) { _exit(code); }