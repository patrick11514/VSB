#include <stdbool.h>

#include "gthr.h"
#include "gthr_platform.h"

struct worker_config {
  const char *name;
  int priority;
};

static void worker(void *arg) {
  struct worker_config *cfg = arg;
  int iteration = 0;

  while (true) {
    gt_platform_print("%s priority=%d iteration=%d BEGINNING\n", cfg->name,
                      cfg->priority, ++iteration);
    gt_uninterruptible_nanosleep(0, 50000000);
    gt_platform_print("%s priority=%d iteration=%d END\n", cfg->name,
                      cfg->priority, ++iteration);
    gt_uninterruptible_nanosleep(0, 50000000);
  }
}

static enum gt_scheduler_mode parse_mode(int argc, char **argv) {
  if (argc < 2)
    return GtSchedulerRR;
  if (gt_platform_strcmp(argv[1], "rr") == 0 ||
      gt_platform_strcmp(argv[1], "--rr") == 0)
    return GtSchedulerRR;
  if (gt_platform_strcmp(argv[1], "pri") == 0 ||
      gt_platform_strcmp(argv[1], "--priority") == 0)
    return GtSchedulerPriority;
  if (gt_platform_strcmp(argv[1], "ls") == 0 ||
      gt_platform_strcmp(argv[1], "--lottery") == 0)
    return GtSchedulerLottery;

  gt_platform_print("usage: %s [rr|pri|ls]\n", argv[0]);
  gt_platform_exit(1);
}

static const char *mode_name(enum gt_scheduler_mode mode) {
  switch (mode) {
  case GtSchedulerRR:
    return "RR";
  case GtSchedulerPriority:
    return "PRI";
  case GtSchedulerLottery:
    return "LS";
  default:
    return "?";
  }
}

int main(int argc, char **argv) {
  static const struct worker_config configs[] = {
      {"A", 0},
      {"B", 2},
      {"C", 4},
      {"D", 7},
  };
  enum gt_scheduler_mode mode;
  int i;

  mode = parse_mode(argc, argv);
  gt_set_scheduler_mode(mode);
  gt_platform_print("Selected scheduler: %s\n", mode_name(mode));
  scheduler_init();
  for (i = 0; i < (int)(sizeof(configs) / sizeof(configs[0])); ++i) {
    if (gt_create_with_priority(worker, (void *)&configs[i],
                                configs[i].priority) != 0) {
      gt_platform_print("failed to create worker %d\n", i);
      return 1;
    }
  }
  gt_exit(1);
  return 0;
}