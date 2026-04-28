#ifndef GTHR_H
#define GTHR_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

enum {
  MaxGThreads = 5,
  StackSize = 0x400000,
  GtMaxPriority = 10,
};

enum gt_scheduler_mode {
  GtSchedulerRR = 0,
  GtSchedulerPriority = 1,
  GtSchedulerLottery = 2,
};

struct gt {
  struct gt_context {
    uint64_t rsp;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
  } ctx;

  enum {
    Unused,
    Running,
    Ready,
  } state;

  void (*entry)(void *);
  void *arg;
  int priority;
  int base_priority;
  int tickets;
  int starvation_passes;

  uint64_t run_total_us;
  uint64_t wait_total_us;
  uint64_t run_min_us;
  uint64_t run_max_us;
  uint64_t wait_min_us;
  uint64_t wait_max_us;
  double run_mean_us;
  double run_m2_us;
  double wait_mean_us;
  double wait_m2_us;
  uint64_t run_samples;
  uint64_t wait_samples;
  uint64_t switches_count;
  uint64_t last_run_start_us;
  uint64_t last_ready_start_us;
};

void gt_init(void);
void gt_return(int ret);
void gt_switch(struct gt_context *old, struct gt_context *new);
bool gt_schedule(void);
void gt_stop(void);
int gt_create(void (*entry)(void *), void *arg);
int gt_create_with_priority(void (*entry)(void *), void *arg, int priority);
void gt_reset_sig(int sig);
void gt_alarm_handle(int sig);
int gt_uninterruptible_nanosleep(time_t sec, long nanosec);
void gt_set_scheduler_mode(enum gt_scheduler_mode mode);
void scheduler_init(void);
void scheduler_tick(void);
void gt_yield(void);
void gt_exit(int code);
bool gt_shutdown_requested(void);

#endif