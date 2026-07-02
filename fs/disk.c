#include "disk.h"
#include "../utils/string.h"

#define FS_DISK_BLOCK_SIZE 64u
#define FS_DISK_BLOCK_COUNT 64u

static uint8_t fs_disk_storage[FS_DISK_BLOCK_SIZE * FS_DISK_BLOCK_COUNT];

void fs_disk_init(void) {
    memset(fs_disk_storage, 0, sizeof(fs_disk_storage));
}

int fs_disk_read(uint32_t block, void *buffer, size_t size) {
    if (block >= FS_DISK_BLOCK_COUNT || buffer == NULL || size > FS_DISK_BLOCK_SIZE) {
        return 0;
    }

    uint8_t *src = fs_disk_storage + (block * FS_DISK_BLOCK_SIZE);
    memcpy(buffer, src, size);
    return 1;
}

int fs_disk_write(uint32_t block, const void *buffer, size_t size) {
    if (block >= FS_DISK_BLOCK_COUNT || buffer == NULL || size > FS_DISK_BLOCK_SIZE) {
        return 0;
    }

    uint8_t *dst = fs_disk_storage + (block * FS_DISK_BLOCK_SIZE);
    memcpy(dst, buffer, size);
    return 1;
}
