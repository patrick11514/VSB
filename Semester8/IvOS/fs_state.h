#ifndef FS_STATE_H
#define FS_STATE_H

#include "drivers/fat_fs.h"

extern FatFileSystem g_fat_fs;
extern PartitionTable g_fat_partitions[4];

#endif // FS_STATE_H