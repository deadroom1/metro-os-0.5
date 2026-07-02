#ifndef FS_DISK_H
#define FS_DISK_H

#include "../lib/types.h"

void fs_disk_init(void);
int fs_disk_read(uint32_t block, void *buffer, size_t size);
int fs_disk_write(uint32_t block, const void *buffer, size_t size);

#endif
