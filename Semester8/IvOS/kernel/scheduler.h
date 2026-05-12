#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include <stdint.h>

enum kt_state { KT_UNUSED = 0, KT_RUNNING = 1, KT_READY = 2, KT_PAUSED = 3 };

void scheduler_init(void);
int scheduler_create(void (*entry)(void *), void *arg, int priority);
int scheduler_get_slot(int tid);
int scheduler_activate(int tid, void (*entry)(void *));
int scheduler_get_pid(int tid);
void scheduler_tick(void);
void scheduler_yield(void);
void scheduler_exit(int code);
void scheduler_dump(void);

/* Pause/resume/kill operations */
int scheduler_pause(int pid);
int scheduler_resume(int pid);
int scheduler_kill(int pid);
void scheduler_list(void);

/* Set scheduler mode: 0=RR, 1=Priority */
void scheduler_set_mode(int mode);

/* Return current thread index or -1 */
int scheduler_current_index(void);

#endif
