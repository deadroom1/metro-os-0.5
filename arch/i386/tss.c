#include "tss.h"

static tss_entry_t tss_entry;

void tss_init(void) {
    tss_entry.ss0 = 0x10;
    tss_entry.esp0 = 0x0;
    tss_entry.iomap_base = sizeof(tss_entry_t);
}
