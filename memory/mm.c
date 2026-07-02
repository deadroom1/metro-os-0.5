#include "mm.h"

typedef struct mem_block {
    size_t size;
    unsigned int free;
    struct mem_block *next;
} mem_block_t;

static uint8_t heap[METRO_HEAP_SIZE];
static mem_block_t *first_block = (void *)0;

static size_t align_size(size_t size) {
    if (size % 4u) {
        size += 4u - (size % 4u);
    }
    return size;
}

void mm_init(void) {
    first_block = (mem_block_t *)heap;
    first_block->size = METRO_HEAP_SIZE - sizeof(mem_block_t);
    first_block->free = 1;
    first_block->next = (void *)0;
}

void *mm_alloc(size_t size) {
    size = align_size(size + sizeof(mem_block_t));
    if (size == 0) {
        return (void *)0;
    }

    mem_block_t *block = first_block;
    while (block) {
        if (block->free && block->size >= size) {
            if (block->size >= size + sizeof(mem_block_t) + 8u) {
                mem_block_t *split = (mem_block_t *)((uint8_t *)block + size);
                split->size = block->size - size;
                split->free = 1;
                split->next = block->next;
                block->size = size;
                block->next = split;
            }
            block->free = 0;
            return (void *)((uint8_t *)block + sizeof(mem_block_t));
        }
        block = block->next;
    }

    return (void *)0;
}

void mm_free(void *ptr) {
    if (!ptr) {
        return;
    }

    mem_block_t *block = (mem_block_t *)((uint8_t *)ptr - sizeof(mem_block_t));
    block->free = 1;

    mem_block_t *next = block->next;
    while (next && next->free) {
        block->size += next->size + sizeof(mem_block_t);
        block->next = next->next;
        next = block->next;
    }
}

size_t mm_available(void) {
    size_t available = 0;
    mem_block_t *block = first_block;
    while (block) {
        if (block->free) {
            available += block->size;
        }
        block = block->next;
    }
    return available;
}
