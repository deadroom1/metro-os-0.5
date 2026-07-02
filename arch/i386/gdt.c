#include "gdt.h"

static gdt_entry_t gdt[3];
gdt_ptr_t gdt_ptr;

extern void gdt_flush(uint32_t);

void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= gran & 0xF0;
    gdt[index].access = access;
}

void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt[0]) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t)&gdt_ptr);
}
