#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>


void systick_init(void);
uint32_t systick_elapsed(uint32_t start, uint32_t end);

#endif