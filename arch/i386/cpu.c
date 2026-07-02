#include "cpu.h"
#include "gdt.h"

void cpu_init(void) {
    gdt_init();
}

void cpu_halt(void) {
    __asm__ volatile ("cli; hlt");
}
