#ifndef GTHR_STRUCT_H
#define GTHR_STRUCT_H

#include "gthr.h"

struct gt gt_table[MaxGThreads];
struct gt *gt_current;
enum gt_scheduler_mode gt_scheduler_mode;
uint8_t gt_stacks[MaxGThreads][StackSize];

#endif