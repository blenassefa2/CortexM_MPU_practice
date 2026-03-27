#include "../include/shared.h"

volatile uint32_t shared_counter = 0;
volatile uint32_t ready_cpu0     = 0;
volatile uint32_t ready_cpu1     = 0;


volatile uint32_t done_cpu0 = 0;
volatile uint32_t done_cpu1 = 0;