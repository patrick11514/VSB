#include "scheduler.h"
#include "../config.h"
#include "../lib/string.h"
#include "../lib/types.h"
#include "../partitions.h"
#include <assert.h>
#include <stdint.h>

enum { MaxThreads = MAX_PROCESSES };

#define EXEC_SLOT 0
#define APP_MAX_SIZE (512 * 1024) /* 512 KB */

#define IS_APP(t) ((uint32_t)(t)->entry == (uint32_t)PROC_BASE(EXEC_SLOT))

volatile int kt_prev_slot = -1;
volatile int kt_next_slot = -1;

struct kt {
  int state; /* enum kt_state */
  int pid;
  char name[32];
  void (*entry)(void *);
  void *arg;
  int priority;
  int base_priority;
  int starvation_passes;
  uint32_t esp;
  uint32_t base;
  uint32_t stack_top;
  int slot;
};

static struct kt kt_table[MaxThreads];
static int kt_current = -1;
static int scheduler_mode = 0; /* 0=RR, 1=Priority */
static uint32_t next_pid = 1;

extern void kt_thread_start(void);

void scheduler_init(void) {
  memset(kt_table, 0, sizeof(kt_table));
  for (int i = 0; i < MaxThreads; ++i)
    kt_table[i].state = KT_UNUSED;
  partitions_init();
  kt_current = -1;
}

static int find_rr_next(void) {
  int start = kt_current < 0 ? 0 : kt_current;
  for (int i = 0; i < MaxThreads; ++i) {
    int idx = (start + 1 + i) % MaxThreads;
    if (kt_table[idx].state == KT_READY) {
      return idx;
    }
  }
  return -1;
}

static int find_pid_by_tid(int tid) {
  if (tid < 0 || tid >= MaxThreads)
    return -1;
  return kt_table[tid].pid;
}

static int find_tid_by_pid(int pid) {
  for (int i = 0; i < MaxThreads; ++i) {
    if (kt_table[i].pid == pid && kt_table[i].state != KT_UNUSED)
      return i;
  }
  return -1;
}

static int find_priority_next(void) {
  int best = -1;
  int best_prio = 9999;
  for (int i = 0; i < MaxThreads; ++i) {
    if (kt_table[i].state != KT_READY)
      continue;
    if (kt_table[i].priority < best_prio) {
      best_prio = kt_table[i].priority;
      best = i;
    }
  }
  return best;
}

int scheduler_pause(int pid) {
  int tid = find_tid_by_pid(pid);
  if (tid < 0)
    return -1;
  if (kt_table[tid].state == KT_READY || kt_table[tid].state == KT_RUNNING) {
    kt_table[tid].state = KT_PAUSED;
    return 0;
  }
  return -1;
}

int scheduler_resume(int pid) {
  int tid = find_tid_by_pid(pid);
  if (tid < 0)
    return -1;
  if (kt_table[tid].state == KT_PAUSED) {
    kt_table[tid].state = KT_READY;
    return 0;
  }
  return -1;
}

int scheduler_kill(int pid) {
  int tid = find_tid_by_pid(pid);
  if (tid < 0)
    return -1;
  if (kt_table[tid].slot >= 0) {
    partitions_free(kt_table[tid].slot);
  }
  kt_table[tid].state = KT_UNUSED;
  kt_table[tid].pid = 0;
  kt_table[tid].entry = NULL;
  return 0;
}

static void age_priority(int selected) {
  for (int i = 0; i < MaxThreads; ++i) {
    if (i == selected)
      continue;
    if (kt_table[i].state != KT_READY)
      continue;
    kt_table[i].starvation_passes++;
    if (kt_table[i].starvation_passes >= MaxThreads &&
        kt_table[i].priority > 0) {
      kt_table[i].priority--;
      kt_table[i].starvation_passes = 0;
    }
  }
}

int scheduler_create(void (*entry)(void *), void *arg, int priority) {
  int idx = -1;
  for (int i = 0; i < MaxThreads; ++i) {
    if (kt_table[i].state == KT_UNUSED && kt_table[i].pid == 0) {
      idx = i;
      break;
    }
  }
  if (idx < 0)
    return -1;

  int slot = partitions_alloc();
  if (slot < 0)
    return -1;

  struct kt *t = &kt_table[idx];
  t->state = KT_UNUSED;
  t->pid = next_pid++;
  t->entry = NULL;
  t->arg = arg;
  t->priority = priority;
  t->base_priority = priority;
  t->starvation_passes = 0;
  t->base = PROC_BASE(slot);
  t->stack_top = PROC_STACK_TOP(slot);
  t->slot = slot;
  t->esp = 0;
  memset(t->name, 0, sizeof(t->name));

  return idx;
}

int scheduler_current_index(void) { return kt_current; }

int scheduler_get_slot(int tid) {
  if (tid < 0 || tid >= MaxThreads)
    return -1;
  return kt_table[tid].slot;
}

int scheduler_get_pid(int tid) {
  if (tid < 0 || tid >= MaxThreads)
    return -1;
  return kt_table[tid].pid;
}

int scheduler_activate(int tid, void (*entry)(void *)) {
  if (tid < 0 || tid >= MaxThreads)
    return -1;
  struct kt *t = &kt_table[tid];
  if (t->slot < 0)
    return -1;

  t->entry = entry;

  if (!IS_APP(t)) {
    /* 1. Kernel/CLI Thread */
    uint32_t sp = t->stack_top;

    sp -= 4;
    *(uint32_t *)sp = (uint32_t)kt_thread_start;
    sp -= 4;
    *(uint32_t *)sp = 0x200; /* Initial EFLAGS: Interrupts Enabled */
    for (int i = 0; i < 8; i++) {
      sp -= 4;
      *(uint32_t *)sp = 0;
    }

    t->esp = sp;
  } else {
    /* 2. User App */
    uint32_t storage_sp = t->stack_top;

    storage_sp -= 4;
    *(uint32_t *)storage_sp = (uint32_t)kt_thread_start;
    storage_sp -= 4;
    *(uint32_t *)storage_sp = 0x200; /* Initial EFLAGS: Interrupts Enabled */
    for (int i = 0; i < 8; i++) {
      storage_sp -= 4;
      *(uint32_t *)storage_sp = 0;
    }

    uint32_t offset = storage_sp - (uint32_t)PROC_BASE(t->slot);
    t->esp = (uint32_t)PROC_BASE(EXEC_SLOT) + offset;
  }

  t->state = KT_READY;
  return 0;
}

void scheduler_set_name(int tid, const char *name) {
  if (tid < 0 || tid >= MaxThreads)
    return;
  if (name) {
    int len = 0;
    while (len < 31 && name[len])
      len++;
    for (int i = 0; i <= len; i++)
      kt_table[tid].name[i] = name[i];
  }
}

void scheduler_set_mode(int mode) { scheduler_mode = mode; }

void scheduler_list(void) {
  extern void vga_print(const char *);
  extern void vga_putchar(char c);
  int count = 0;
  for (int i = 0; i < MaxThreads; ++i) {
    struct kt *t = &kt_table[i];
    if (t->state == KT_UNUSED)
      continue;
    if (count == 0) {
      vga_print("PID  NAME                              STATE\n");
    }

    if (t->pid == 0)
      vga_print("0");
    else {
      char pid_buf[16];
      int pos = 0;
      int tmp = t->pid;
      while (tmp > 0) {
        pid_buf[pos++] = '0' + (tmp % 10);
        tmp /= 10;
      }
      for (int j = pos - 1; j >= 0; j--)
        vga_putchar(pid_buf[j]);
    }

    vga_print(" ");
    vga_print(t->name[0] ? t->name : "(unnamed)");
    vga_print(" ");
    if (t->state == KT_READY)
      vga_print("READY");
    else if (t->state == KT_RUNNING)
      vga_print("RUNNING");
    else if (t->state == KT_PAUSED)
      vga_print("PAUSED");
    else
      vga_print("UNKNOWN");
    vga_print("\n");
    count++;
  }
  if (count == 0)
    vga_print("No processes.\n");
}

void kt_do_memory_swap(void) {
  if (kt_prev_slot > EXEC_SLOT) {
    memcpy((void *)PROC_BASE(kt_prev_slot), (void *)PROC_BASE(EXEC_SLOT),
           APP_MAX_SIZE);
  }
  if (kt_next_slot > EXEC_SLOT) {
    memcpy((void *)PROC_BASE(EXEC_SLOT), (void *)PROC_BASE(kt_next_slot),
           APP_MAX_SIZE);
  }
}

extern void kt_switch(uint32_t *old_esp_ptr, uint32_t new_esp_val);

static void do_switch(int next_idx) {
  __asm__ volatile("cli");

  if (next_idx < 0) {
    __asm__ volatile("sti");
    return;
  }
  int prev = kt_current;
  kt_current = next_idx;
  if (prev == next_idx) {
    __asm__ volatile("sti");
    return;
  }

  uint32_t *old_esp_ptr = NULL;
  uint32_t new_esp = kt_table[next_idx].esp;

  /* Prepare the slot variables for the assembly trampoline */
  kt_prev_slot = -1;
  if (prev >= 0) {
    if (kt_table[prev].state == KT_RUNNING) {
      kt_table[prev].state = KT_READY;
    }
    old_esp_ptr = &kt_table[prev].esp;
    if (IS_APP(&kt_table[prev])) {
      kt_prev_slot = kt_table[prev].slot;
    }
  }

  kt_table[next_idx].state = KT_RUNNING;
  kt_next_slot = -1;
  if (IS_APP(&kt_table[next_idx])) {
    kt_next_slot = kt_table[next_idx].slot;
  }

  /* Perform the CPU switch (which will automatically call the memory swap) */
  if (old_esp_ptr)
    kt_switch(old_esp_ptr, new_esp);
  else {
    uint32_t tmp = new_esp;
    kt_switch(&tmp, new_esp);
  }

  __asm__ volatile("sti");
}

void scheduler_tick(void) {
  int next = -1;
  if (scheduler_mode == 1)
    next = find_priority_next();
  else
    next = find_rr_next();

  if (next == -1)
    return;
  age_priority(next);
  do_switch(next);
}

void scheduler_yield(void) {
  if (kt_current >= 0 && kt_table[kt_current].state == KT_RUNNING) {
    kt_table[kt_current].state = KT_READY;
  }
  scheduler_tick();
}

void scheduler_exit(int code) {
  if (kt_current >= 0) {
    int exiting_pid = kt_table[kt_current].pid;
    if (kt_table[kt_current].slot >= 0) {
      partitions_free(kt_table[kt_current].slot);
    }
    kt_table[kt_current].state = KT_UNUSED;
    kt_table[kt_current].pid = 0;
    kt_table[kt_current].entry = NULL;
    kt_current = -1;

    extern void cli_app_exited(int pid);
    cli_app_exited(exiting_pid);
  }
  scheduler_tick();
}

void kt_thread_start(void) {
  int idx = kt_current;
  if (idx < 0)
    return;
  struct kt *t = &kt_table[idx];

  __asm__ volatile("sti");

  if (t->entry)
    t->entry(t->arg);

  scheduler_exit(0);
  while (1) {
  }
}