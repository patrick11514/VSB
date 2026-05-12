#ifndef GTHR_CONFIG_H
#define GTHR_CONFIG_H

#include <stdint.h>

/* Memory model constants for static partitions */
#define KERNEL_BASE 0x00001000
#define PROC_BASE_START 0x00100000
#define PROC_SLOT_SIZE 0x00010000 /* 64 KB */
#define MAX_PROCESSES 8

#define PROC_BASE(i) (PROC_BASE_START + ((i) * PROC_SLOT_SIZE))
#define PROC_STACK_TOP(i) (PROC_BASE(i) + PROC_SLOT_SIZE - 4)

#endif
