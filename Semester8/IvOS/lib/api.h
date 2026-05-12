#ifndef OS_API_H
#define OS_API_H

#include "types.h"

typedef struct {
  int (*getchar)(void);
  void (*print)(const char *);
  void (*exit)(int code);
  void (*yield)(void);
  void (*serial_print)(const char *);
} os_api_t;

#endif