#include "idt.h"

static idt_entry_t idt[METRO_IDT_ENTRIES];
static idt_ptr_t idt_ptr;

static void (*isr_handlers[32])(void) = {0};

static void default_isr_handler(void) {
    for (;;);
}

static void idt_dispatch(uint8_t index) {
    if (index < 32 && isr_handlers[index]) {
        isr_handlers[index]();
        return;
    }
    default_isr_handler();
}

void isr0(void) { idt_dispatch(0); }
void isr1(void) { idt_dispatch(1); }
void isr2(void) { idt_dispatch(2); }
void isr3(void) { idt_dispatch(3); }
void isr4(void) { idt_dispatch(4); }
void isr5(void) { idt_dispatch(5); }
void isr6(void) { idt_dispatch(6); }
void isr7(void) { idt_dispatch(7); }
void isr8(void) { idt_dispatch(8); }
void isr9(void) { idt_dispatch(9); }
void isr10(void) { idt_dispatch(10); }
void isr11(void) { idt_dispatch(11); }
void isr12(void) { idt_dispatch(12); }
void isr13(void) { idt_dispatch(13); }
void isr14(void) { idt_dispatch(14); }
void isr15(void) { idt_dispatch(15); }
void isr16(void) { idt_dispatch(16); }
void isr17(void) { idt_dispatch(17); }
void isr18(void) { idt_dispatch(18); }
void isr19(void) { idt_dispatch(19); }
void isr20(void) { idt_dispatch(20); }
void isr21(void) { idt_dispatch(21); }
void isr22(void) { idt_dispatch(22); }
void isr23(void) { idt_dispatch(23); }
void isr24(void) { idt_dispatch(24); }
void isr25(void) { idt_dispatch(25); }
void isr26(void) { idt_dispatch(26); }
void isr27(void) { idt_dispatch(27); }
void isr28(void) { idt_dispatch(28); }
void isr29(void) { idt_dispatch(29); }
void isr30(void) { idt_dispatch(30); }
void isr31(void) { idt_dispatch(31); }

void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[index].offset_low = base & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = flags;
    idt[index].offset_high = (base >> 16) & 0xFFFF;
}

void idt_install_handler(uint8_t index, void (*handler)(void)) {
    if (index < 32) {
        isr_handlers[index] = handler;
    }
}

void idt_init(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * METRO_IDT_ENTRIES) - 1;
    idt_ptr.base = (uint32_t)&idt;

    for (uint16_t i = 0; i < METRO_IDT_ENTRIES; i++) {
        idt_set_gate(i, (uint32_t)default_isr_handler, 0x08, 0x8E);
    }

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    __asm__ volatile("lidt (%0)" : : "r"(&idt_ptr));
}
