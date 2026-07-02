#include "paging.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_init(void) {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 0x3;
        page_directory[i] = 0;
    }

    page_directory[0] = (uint32_t)first_page_table | 0x3;
    __asm__ volatile ("mov %0, %%cr3" : : "r"(page_directory));
    __asm__ volatile ("mov %%cr0, %%eax; or $0x80000000, %%eax; mov %%eax, %%cr0" : : : "eax");
}

void paging_map_page(uint32_t virt, uint32_t phys) {
    (void)virt;
    (void)phys;
}
