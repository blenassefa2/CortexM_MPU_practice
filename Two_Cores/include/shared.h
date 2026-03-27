#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

#define ITERATIONS 2000000

extern volatile uint32_t shared_counter;
extern volatile uint32_t ready_cpu0;
extern volatile uint32_t ready_cpu1;

// to get clean read after both are done
extern volatile uint32_t done_cpu0;
extern volatile uint32_t done_cpu1;
#endif