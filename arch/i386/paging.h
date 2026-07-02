#ifndef ARCH_I386_PAGING_H
#define ARCH_I386_PAGING_H

#include "../../lib/types.h"

void paging_init(void);
void paging_map_page(uint32_t virt, uint32_t phys);

#endif
