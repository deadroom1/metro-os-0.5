#include "inode.h"
#include "disk.h"
#include "../utils/string.h"

#define FS_MAX_ENTRIES 16u
#define FS_MAX_NAME_LEN 16u
#define FS_BLOCK_SIZE 64u
#define FS_ROOT_BLOCK 0u

typedef struct {
    char name[FS_MAX_NAME_LEN];
    uint8_t type;
    uint32_t block;
    uint32_t size;
} fs_entry_t;

typedef struct {
    fs_entry_t entries[FS_MAX_ENTRIES];
    uint32_t entry_count;
} fs_root_t;

static fs_root_t root_state;

static int fs_same_name(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static int fs_find_entry(const char *name, fs_entry_t **out_entry) {
    for (uint32_t i = 0; i < root_state.entry_count; i++) {
        if (fs_same_name(root_state.entries[i].name, name)) {
            if (out_entry) {
                *out_entry = &root_state.entries[i];
            }
            return 1;
        }
    }
    return 0;
}

static int fs_save_root(void) {
    return fs_disk_write(FS_ROOT_BLOCK, &root_state, sizeof(root_state));
}

static int fs_load_root(void) {
    return fs_disk_read(FS_ROOT_BLOCK, &root_state, sizeof(root_state));
}

void fs_init(void) {
    memset(&root_state, 0, sizeof(root_state));
    fs_disk_init();

    if (!fs_load_root()) {
        memset(&root_state, 0, sizeof(root_state));
    }

    if (root_state.entry_count == 0) {
        fs_create_dir("/");
    }
}

int fs_create_dir(const char *path) {
    if (!path || path[0] == 0) {
        return 0;
    }

    if (fs_exists(path)) {
        return 0;
    }

    if (root_state.entry_count >= FS_MAX_ENTRIES) {
        return 0;
    }

    fs_entry_t *entry = &root_state.entries[root_state.entry_count++];
    memset(entry, 0, sizeof(*entry));
    strncpy(entry->name, path, FS_MAX_NAME_LEN - 1);
    entry->type = FS_INODE_DIR;
    entry->block = FS_ROOT_BLOCK + 1 + root_state.entry_count;
    entry->size = 0;
    return fs_save_root();
}

int fs_create_file(const char *path, const char *content) {
    if (!path || !content || path[0] == 0) {
        return 0;
    }

    if (fs_exists(path)) {
        return 0;
    }

    if (root_state.entry_count >= FS_MAX_ENTRIES) {
        return 0;
    }

    fs_entry_t *entry = &root_state.entries[root_state.entry_count++];
    memset(entry, 0, sizeof(*entry));
    strncpy(entry->name, path, FS_MAX_NAME_LEN - 1);
    entry->type = FS_INODE_FILE;
    entry->block = FS_ROOT_BLOCK + 1 + root_state.entry_count;
    entry->size = strlen(content);

    if (!fs_disk_write(entry->block, content, entry->size)) {
        return 0;
    }

    return fs_save_root();
}

int fs_remove(const char *path) {
    if (!path || path[0] == 0) {
        return 0;
    }

    for (uint32_t i = 0; i < root_state.entry_count; i++) {
        if (fs_same_name(root_state.entries[i].name, path)) {
            for (uint32_t j = i; j + 1 < root_state.entry_count; j++) {
                root_state.entries[j] = root_state.entries[j + 1];
            }
            root_state.entry_count--;
            return fs_save_root();
        }
    }
    return 0;
}

int fs_read_file(const char *path, char *buffer, size_t buffer_size, size_t *out_size) {
    fs_entry_t *entry = NULL;
    if (!path || !buffer || buffer_size == 0 || !fs_find_entry(path, &entry)) {
        return 0;
    }

    if (entry->type != FS_INODE_FILE) {
        return 0;
    }

    size_t size = entry->size < buffer_size - 1 ? entry->size : buffer_size - 1;
    if (!fs_disk_read(entry->block, buffer, size)) {
        return 0;
    }

    buffer[size] = 0;
    if (out_size) {
        *out_size = size;
    }
    return 1;
}

int fs_write_file(const char *path, const char *buffer, size_t size) {
    fs_entry_t *entry = NULL;
    if (!path || !buffer || size == 0 || !fs_find_entry(path, &entry)) {
        return 0;
    }

    if (entry->type != FS_INODE_FILE) {
        return 0;
    }

    entry->size = size;
    if (!fs_disk_write(entry->block, buffer, size)) {
        return 0;
    }
    return fs_save_root();
}

int fs_list_dir(const char *path, char *buffer, size_t buffer_size) {
    (void)path;
    size_t offset = 0;
    for (uint32_t i = 0; i < root_state.entry_count; i++) {
        size_t len = strlen(root_state.entries[i].name);
        if (offset + len + 1 >= buffer_size) {
            break;
        }
        memcpy(buffer + offset, root_state.entries[i].name, len);
        offset += len;
        buffer[offset++] = '\n';
    }
    if (offset > 0 && offset < buffer_size) {
        buffer[offset] = 0;
    }
    return (int)offset;
}

int fs_is_dir(const char *path) {
    fs_entry_t *entry = NULL;
    if (!path || !fs_find_entry(path, &entry)) {
        return 0;
    }
    return entry->type == FS_INODE_DIR;
}

int fs_exists(const char *path) {
    fs_entry_t *entry = NULL;
    return fs_find_entry(path, &entry);
}
