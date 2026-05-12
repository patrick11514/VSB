#include "scheduler.h"
#include "../config.h"
#include "../partitions.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <assert.h>

enum { MaxThreads = MAX_PROCESSES };

struct kt {
    int state; /* enum kt_state */
    void (*entry)(void *);
    void *arg;
    int priority;
    int base_priority;
    int starvation_passes;
    uint32_t esp;
    uint32_t base;
    uint32_t stack_top;
};

static struct kt kt_table[MaxThreads];
static int kt_current = -1;
static int scheduler_mode = 0; /* 0=RR,1=Priority */

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
        if (kt_table[idx].state == KT_READY) return idx;
    }
    return -1;
}

static int find_priority_next(void) {
    int best = -1;
    int best_prio = 9999;
    for (int i = 0; i < MaxThreads; ++i) {
        if (kt_table[i].state != KT_READY) continue;
        if (kt_table[i].priority < best_prio) {
            best_prio = kt_table[i].priority;
            best = i;
        }
    }
    return best;
}

static void age_priority(int selected) {
    for (int i = 0; i < MaxThreads; ++i) {
        if (i == selected) continue;
        if (kt_table[i].state != KT_READY) continue;
        kt_table[i].starvation_passes++;
        if (kt_table[i].starvation_passes >= MaxThreads && kt_table[i].priority > 0) {
            kt_table[i].priority--;
            kt_table[i].starvation_passes = 0;
        }
    }
}

int scheduler_create(void (*entry)(void *), void *arg, int priority) {
    int idx = -1;
    for (int i = 0; i < MaxThreads; ++i) {
        if (kt_table[i].state == KT_UNUSED) { idx = i; break; }
    }
    if (idx < 0) return -1;

    /* allocate partition for this thread */
    int slot = partitions_alloc();
    if (slot < 0) return -1;

    struct kt *t = &kt_table[idx];
    t->state = KT_READY;
    t->entry = entry;
    t->arg = arg;
    t->priority = priority;
    t->base_priority = priority;
    t->starvation_passes = 0;
    t->base = PROC_BASE(slot);
    t->stack_top = PROC_STACK_TOP(slot);

    /* prepare initial stack: place address of kt_thread_start as return address */
    uint32_t sp = t->stack_top - 4;
    uint32_t *p = (uint32_t *)sp;
    *p = (uint32_t)kt_thread_start;
    t->esp = sp;

    return idx;
}

int scheduler_current_index(void) { return kt_current; }

void scheduler_set_mode(int mode) { scheduler_mode = mode; }

void scheduler_dump(void) {
    for (int i = 0; i < MaxThreads; ++i) {
        struct kt *t = &kt_table[i];
        /* tiny dump to serial/vga if available; for now use placeholder */
    }
}

/* Forward declaration of arch switch */
extern void kt_switch(uint32_t *old_esp_ptr, uint32_t new_esp_val);

static void do_switch(int next_idx) {
    if (next_idx < 0) return;
    int prev = kt_current;
    kt_current = next_idx;
    if (prev == next_idx) return;

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

    if (next == -1) return;
    age_priority(next);
    do_switch(next);
}

void scheduler_yield(void) {
    /* mark current ready and pick next */
    if (kt_current >= 0) kt_table[kt_current].state = KT_READY;
    scheduler_tick();
}

void scheduler_exit(int code) {
    if (kt_current >= 0) {
        /* free partition */
        uint32_t base = kt_table[kt_current].base;
        int slot = (base - PROC_BASE_START) / PROC_SLOT_SIZE;
        partitions_free(slot);
        kt_table[kt_current].state = KT_UNUSED;
        kt_table[kt_current].entry = NULL;
        kt_current = -1;
    }
    /* schedule next */
    scheduler_tick();
}

/* Thread bootstrap called when a new thread first starts */
void kt_thread_start(void) {
    int idx = kt_current;
    if (idx < 0) return;
    struct kt *t = &kt_table[idx];
    if (t->entry) t->entry(t->arg);
    scheduler_exit(0);
    while (1) {}
}
