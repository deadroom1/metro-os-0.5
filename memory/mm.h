#ifndef MM_H
#define MM_H

#include "../lib/types.h"
#include "../config/config.h"

void mm_init(void);
void *mm_alloc(size_t size);
void mm_free(void *ptr);
size_t mm_available(void);

#endif
