#include "stdlib.h"

static char *reverse(char *buffer, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char tmp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = tmp;
        start++;
        end--;
    }
    return buffer;
}

char *itoa(int value, char *buffer, int base) {
    int negative = 0;
    int i = 0;

    if (base < 2 || base > 16) {
        buffer[0] = '\0';
        return buffer;
    }

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (value < 0 && base == 10) {
        negative = 1;
        value = -value;
    }

    while (value > 0) {
        int rem = value % base;
        buffer[i++] = (rem < 10) ? ('0' + rem) : ('A' + rem - 10);
        value /= base;
    }

    if (negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';
    return reverse(buffer, i);
}

char *utoa(unsigned int value, char *buffer, int base) {
    int i = 0;

    if (base < 2 || base > 16) {
        buffer[0] = '\0';
        return buffer;
    }

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    while (value > 0) {
        unsigned int rem = value % (unsigned int)base;
        buffer[i++] = (rem < 10u) ? ('0' + rem) : ('A' + rem - 10u);
        value /= (unsigned int)base;
    }

    buffer[i] = '\0';
    return reverse(buffer, i);
}
