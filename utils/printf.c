#include "printf.h"
#include "string.h"
#include "stdlib.h"
#include "../drivers/vga.h"
#include "../lib/types.h"
#include "../lib/stdarg.h"

static void print_dec(unsigned int value) {
    char buffer[16];
    utoa(value, buffer, 10);
    vga_puts(buffer);
}

static void print_hex(unsigned int value) {
    char buffer[16];
    utoa(value, buffer, 16);
    vga_puts(buffer);
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            vga_putchar(*fmt);
            fmt++;
            continue;
        }

        fmt++;
        switch (*fmt) {
            case 's': {
                const char *s = va_arg(args, const char *);
                vga_puts(s ? s : "(null)");
                break;
            }
            case 'd': {
                unsigned int value = va_arg(args, unsigned int);
                print_dec(value);
                break;
            }
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                print_hex(value);
                break;
            }
            case '%':
                vga_putchar('%');
                break;
            default:
                vga_putchar('%');
                vga_putchar(*fmt);
                break;
        }
        fmt++;
    }

    va_end(args);
}
