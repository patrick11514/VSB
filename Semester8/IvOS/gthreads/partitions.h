#ifndef GTHR_PARTITIONS_H
#define GTHR_PARTITIONS_H

#include <stdint.h>

void partitions_init(void);
int partitions_alloc(void); /* returns slot index or -1 */
void partitions_free(int idx);
int partitions_is_used(int idx);

#endif
