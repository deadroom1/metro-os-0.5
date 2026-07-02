#include "irq.h"
#include "idt.h"

static void (*irq_handlers[16])(void) = {0};

static void irq_remap(void) {
    __asm__ volatile("outb %%al, $0x20" : : "a"(0x11));
    __asm__ volatile("outb %%al, $0xA0" : : "a"(0x11));
    __asm__ volatile("outb %%al, $0x21" : : "a"(0x20));
    __asm__ volatile("outb %%al, $0xA1" : : "a"(0x28));
    __asm__ volatile("outb %%al, $0x21" : : "a"(0x04));
    __asm__ volatile("outb %%al, $0xA1" : : "a"(0x02));
    __asm__ volatile("outb %%al, $0x21" : : "a"(0x01));
    __asm__ volatile("outb %%al, $0xA1" : : "a"(0x01));
    __asm__ volatile("outb %%al, $0x21" : : "a"(0x00));
    __asm__ volatile("outb %%al, $0xA1" : : "a"(0x00));
}

static void irq_dispatch(uint8_t irq) {
    if (irq < 16 && irq_handlers[irq]) {
        irq_handlers[irq]();
    }
    irq_ack(irq);
}

void irq0(void) { irq_dispatch(0); }
void irq1(void) { irq_dispatch(1); }
void irq2(void) { irq_dispatch(2); }
void irq3(void) { irq_dispatch(3); }
void irq4(void) { irq_dispatch(4); }
void irq5(void) { irq_dispatch(5); }
void irq6(void) { irq_dispatch(6); }
void irq7(void) { irq_dispatch(7); }
void irq8(void) { irq_dispatch(8); }
void irq9(void) { irq_dispatch(9); }
void irq10(void) { irq_dispatch(10); }
void irq11(void) { irq_dispatch(11); }
void irq12(void) { irq_dispatch(12); }
void irq13(void) { irq_dispatch(13); }
void irq14(void) { irq_dispatch(14); }
void irq15(void) { irq_dispatch(15); }

void irq_install_handler(uint8_t irq, void (*handler)(void)) {
    if (irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_ack(uint8_t irq) {
    if (irq >= 8) {
        __asm__ volatile("outb %%al, $0xA0" : : "a"(0x20));
    }
    __asm__ volatile("outb %%al, $0x20" : : "a"(0x20));
}

void irq_init(void) {
    irq_remap();

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    __asm__ volatile("cli");
}
