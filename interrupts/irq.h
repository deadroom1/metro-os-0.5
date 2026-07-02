#ifndef IRQ_H
#define IRQ_H

#include "../lib/types.h"

void irq_init(void);
void irq_install_handler(uint8_t irq, void (*handler)(void));
void irq_ack(uint8_t irq);

#endif
