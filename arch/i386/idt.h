#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include "../../lib/types.h"

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);
void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags);
void idt_install_handler(uint8_t index, void (*handler)(void));

#endif
