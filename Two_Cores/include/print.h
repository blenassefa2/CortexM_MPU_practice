#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
static void sh_puts(const char *str) {
    register uint32_t r0 __asm("r0") = 0x04;
    register const char *r1 __asm("r1") = str;
    asm("bkpt 0xAB" :: "r" (r0), "r" (r1) : "memory");
}

static void sh_putx(uint32_t value){
    char buf[9];   
    for (int i = 0; i < 8; i++) {
        uint32_t nibble = (value >> (28 - 4 * i)) & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
    }
    buf[8] = '\0';
    sh_puts(buf);
}

#endif /*PRINT_H */