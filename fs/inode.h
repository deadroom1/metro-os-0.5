#ifndef FS_INODE_H
#define FS_INODE_H

#include "../lib/types.h"

typedef enum {
    FS_INODE_FILE = 1,
    FS_INODE_DIR = 2
} fs_inode_type_t;

void fs_init(void);
int fs_create_file(const char *path, const char *content);
int fs_create_dir(const char *path);
int fs_remove(const char *path);
int fs_read_file(const char *path, char *buffer, size_t buffer_size, size_t *out_size);
int fs_write_file(const char *path, const char *buffer, size_t size);
int fs_list_dir(const char *path, char *buffer, size_t buffer_size);
int fs_is_dir(const char *path);
int fs_exists(const char *path);

#endif
