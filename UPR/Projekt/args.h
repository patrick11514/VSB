#ifndef ARGS_H
#define ARGS_H

// basic includes
#include <stdbool.h>
#include <sys/cdefs.h>

bool loadArgs(int argc, char **argv, bool *levels, bool *includeDefaultLevels) __attribute_warn_unused_result__;

#endif