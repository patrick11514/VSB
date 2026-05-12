#include "partitions.h"
#include "config.h"

#include <stddef.h>
#include <stdint.h>

struct partition {
  uint32_t base;
  int used;
};

static struct partition parts[MAX_PROCESSES];

void partitions_init(void) {
  for (int i = 0; i < MAX_PROCESSES; ++i) {
    parts[i].base = (uint32_t)PROC_BASE(i);
    parts[i].used = 0;
  }
}

int partitions_alloc(void) {
  for (int i = 0; i < MAX_PROCESSES; ++i) {
    if (!parts[i].used) {
      parts[i].used = 1;
      return i;
    }
  }
  return -1;
}

void partitions_free(int idx) {
  if (idx < 0 || idx >= MAX_PROCESSES)
    return;
  parts[idx].used = 0;
}

int partitions_is_used(int idx) {
  if (idx < 0 || idx >= MAX_PROCESSES)
    return 0;
  return parts[idx].used;
}
