#include "sys_cmd.h"
#include "../drivers/vga.h"
#include "../fs/inode.h"

extern uint32_t kernel_uptime_seconds(void);

static char current_dir[16] = "/";

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
}

void cmd_cpuinfo(void) {
    char vendor[13] = {0};
    uint32_t ebx, ecx, edx;
    __asm__ volatile (
        "cpuid"
        : "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0)
    );
    /* vendor string: EBX EDX ECX */
    for (int i = 0; i < 4; i++) vendor[i]     = (ebx >> (i*8)) & 0xFF;
    for (int i = 0; i < 4; i++) vendor[i+4]   = (edx >> (i*8)) & 0xFF;
    for (int i = 0; i < 4; i++) vendor[i+8]   = (ecx >> (i*8)) & 0xFF;

    vga_set_color(COLOR_LMAGENTA, COLOR_BLACK);
    vga_puts("CPU Vendor: ");
    vga_puts(vendor);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_shutdown(void) {
    vga_set_color(COLOR_LRED, COLOR_BLACK);
    vga_puts("Shutting down...\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);

    /* Try common QEMU/Bochs power-off ports */
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);

    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void cmd_dir(void) {
    char listing[128] = {0};
    vga_set_color(COLOR_LCYAN, COLOR_BLACK);
    vga_puts("Directory of ");
    vga_puts(current_dir);
    vga_putchar('\n');

    if (fs_list_dir(current_dir, listing, sizeof(listing)) > 0) {
        vga_puts(listing);
        vga_putchar('\n');
    } else {
        vga_puts("(empty)\n");
    }

    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_cd(const char *args) {
    if (!args || args[0] == 0) {
        current_dir[0] = '/';
        current_dir[1] = 0;
        return;
    }

    if (args[0] == '/' && args[1] == 0) {
        current_dir[0] = '/';
        current_dir[1] = 0;
        return;
    }

    if (args[0] == '.' && args[1] == '.' && args[2] == 0) {
        if (current_dir[0] == '/' && current_dir[1] == 'b' && current_dir[2] == 'o' && current_dir[3] == 'o' && current_dir[4] == 't' && current_dir[5] == 0) {
            current_dir[0] = '/';
            current_dir[1] = 0;
        }
        return;
    }

    if (args[0] == 'b' && args[1] == 'o' && args[2] == 'o' && args[3] == 't' && args[4] == 0) {
        current_dir[0] = '/';
        current_dir[1] = 'b';
        current_dir[2] = 'o';
        current_dir[3] = 'o';
        current_dir[4] = 't';
        current_dir[5] = 0;
        return;
    }

    vga_set_color(COLOR_LRED, COLOR_BLACK);
    vga_puts("Directory not found\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_cat(const char *args) {
    char buffer[128];
    size_t size = 0;

    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: cat <file>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LCYAN, COLOR_BLACK);
    if (fs_read_file(args, buffer, sizeof(buffer), &size)) {
        vga_puts(buffer);
        vga_putchar('\n');
    } else {
        vga_puts("File not found\n");
    }
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_restart(void) {
    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    vga_puts("Restarting...\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);

    outb(0x64, 0xFE);
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

static void print_u32(uint32_t value) {
    char buffer[16];
    int i = 0;
    if (value == 0) {
        vga_putchar('0');
        return;
    }

    while (value > 0 && i < 15) {
        buffer[i++] = (char)('0' + (value % 10));
        value /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        vga_putchar(buffer[j]);
    }
}

void cmd_uptime(void) {
    uint32_t seconds = kernel_uptime_seconds();

    vga_set_color(COLOR_LGREEN, COLOR_BLACK);
    vga_puts("Uptime: ");
    print_u32(seconds);
    vga_puts(" seconds\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_countdown(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: countdown <seconds>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    int value = 0;
    for (int i = 0; args[i]; i++) {
        if (args[i] < '0' || args[i] > '9') break;
        value = value * 10 + (args[i] - '0');
    }

    if (value <= 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Enter a positive number.\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LCYAN, COLOR_BLACK);
    while (value >= 0) {
        char buffer[16];
        int pos = 0;
        int temp = value;
        if (temp == 0) {
            buffer[pos++] = '0';
        } else {
            int digits[10];
            int digit_count = 0;
            while (temp > 0) {
                digits[digit_count++] = temp % 10;
                temp /= 10;
            }
            for (int j = digit_count - 1; j >= 0; j--) {
                buffer[pos++] = '0' + digits[j];
            }
        }
        buffer[pos] = 0;
        vga_puts(buffer);
        vga_puts("\n");
        value--;
    }
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_banner(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: banner <text>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LMAGENTA, COLOR_BLACK);
    vga_puts("---------------------------------------------------------------\n");
    vga_puts(args);
    vga_puts("\n");
    vga_puts("---------------------------------------------------------------\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_echo(const char *args) {
    if (!args || args[0] == 0) {
        vga_putchar('\n');
        return;
    }

    vga_set_color(COLOR_LGREEN, COLOR_BLACK);
    vga_puts(args);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_touch(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: touch <file>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LCYAN, COLOR_BLACK);
    if (fs_create_file(args, "")) {
        vga_puts("Created file: ");
    } else {
        vga_puts("File exists or create failed: ");
    }
    vga_puts(args);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_locate(void) {
    vga_set_color(COLOR_LMAGENTA, COLOR_BLACK);
    vga_puts(current_dir);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

const char *cmd_get_current_dir(void) {
    return current_dir;
}

void cmd_mkdir(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: mkdir <dir>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LCYAN, COLOR_BLACK);
    if (fs_create_dir(args)) {
        vga_puts("Created directory: ");
    } else {
        vga_puts("Directory exists or create failed: ");
    }
    vga_puts(args);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_rmdir(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: rmdir <dir>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    if (fs_remove(args)) {
        vga_puts("Removed directory: ");
    } else {
        vga_puts("Remove failed: ");
    }
    vga_puts(args);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

void cmd_rm(const char *args) {
    if (!args || args[0] == 0) {
        vga_set_color(COLOR_LRED, COLOR_BLACK);
        vga_puts("Usage: rm <file>\n");
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        return;
    }

    vga_set_color(COLOR_LRED, COLOR_BLACK);
    if (fs_remove(args)) {
        vga_puts("Removed file: ");
    } else {
        vga_puts("Remove failed: ");
    }
    vga_puts(args);
    vga_putchar('\n');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}
