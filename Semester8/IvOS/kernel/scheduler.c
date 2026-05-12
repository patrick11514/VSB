#include "scheduler.h"
#include "../config.h"
#include "../partitions.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

enum { MaxThreads = MAX_PROCESSES };

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
static int scheduler_mode = 0; /* 0=RR,1=Priority */
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
    if (kt_table[idx].state == KT_READY)
      return idx;
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
  extern void serial_print(const char *);
  extern void vga_print(const char *);
  serial_print("[PAUSE_CALL]");
  vga_print("[PAUSE]");
  int tid = find_tid_by_pid(pid);
  if (tid < 0) {
    serial_print("[PAUSE_FAIL_NO_TID]\n");
    return -1;
  }
  serial_print("[PAUSE_OK]\n");
  if (kt_table[tid].state == KT_READY || kt_table[tid].state == KT_RUNNING) {
    kt_table[tid].state = KT_PAUSED;
    return 0;
  }
  return -1;
}

int scheduler_resume(int pid) {
  extern void serial_print(const char *);
  extern void vga_print(const char *);
  serial_print("[RESUME_CALL]");
  vga_print("[RESUME]");
  int tid = find_tid_by_pid(pid);
  if (tid < 0) {
    serial_print("[RESUME_FAIL_NO_TID]\n");
    return -1;
  }
  serial_print("[RESUME_OK]\n");
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

  /* allocate partition for this thread */
  int slot = partitions_alloc();
  if (slot < 0)
    return -1;

  struct kt *t = &kt_table[idx];
  /* do not mark READY yet — caller will activate when code is placed */
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
  if (kt_table[tid].state == KT_UNUSED && kt_table[tid].slot >= 0)
    return kt_table[tid].slot;
  /* if thread was activated, slot still valid */
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
  /* prepare initial stack: place address of kt_thread_start as return address
   */
  uint32_t sp = t->stack_top - 4;
  uint32_t *p = (uint32_t *)sp;
  *p = (uint32_t)kt_thread_start;
  t->esp = sp;
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

void scheduler_dump(void) {
  for (int i = 0; i < MaxThreads; ++i) {
    struct kt *t = &kt_table[i];
    if (t->state != KT_UNUSED) {
      /* list pid, name, state */
    }
  }
}

void scheduler_list(void) {
  /* List all non-unused processes with their PID, name, and state */
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

    /* Print PID as decimal inline */
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

/* Forward declaration of arch switch */
extern void kt_switch(uint32_t *old_esp_ptr, uint32_t new_esp_val);

static void do_switch(int next_idx) {
  if (next_idx < 0)
    return;
  int prev = kt_current;
  kt_current = next_idx;
  if (prev == next_idx)
    return;

  uint32_t *old_esp_ptr = NULL;
  uint32_t new_esp = kt_table[next_idx].esp;
  if (prev >= 0) {
    kt_table[prev].state = KT_READY;
    old_esp_ptr = &kt_table[prev].esp;
  }
  kt_table[next_idx].state = KT_RUNNING;
  /* perform context switch */
  if (old_esp_ptr)
    kt_switch(old_esp_ptr, new_esp);
  else {
    /* First run: no old_esp to save; just set esp and ret into thread */
    uint32_t tmp = new_esp;
    kt_switch(&tmp, new_esp); /* old_esp ptr unused in this path */
  }
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
  /* mark current ready and pick next */
  if (kt_current >= 0)
    kt_table[kt_current].state = KT_READY;
  scheduler_tick();
}

void scheduler_exit(int code) {
  if (kt_current >= 0) {
    /* free partition using stored slot */
    if (kt_table[kt_current].slot >= 0) {
      partitions_free(kt_table[kt_current].slot);
    }
    kt_table[kt_current].state = KT_UNUSED;
    kt_table[kt_current].pid = 0;
    kt_table[kt_current].entry = NULL;
    kt_current = -1;
  }
  /* schedule next */
  scheduler_tick();
}

/* Thread bootstrap called when a new thread first starts */
void kt_thread_start(void) {
  int idx = kt_current;
  if (idx < 0)
    return;
  struct kt *t = &kt_table[idx];
  if (t->entry)
    t->entry(t->arg);
  scheduler_exit(0);
  while (1) {
  }
}
