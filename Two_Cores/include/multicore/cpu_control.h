#ifndef CPU_CONTROL_H
#define CPU_CONTROL_H

#include <stdint.h>
extern uint32_t __Vectors;



uint32_t get_cpu_id(void);
void setup_cpu1();

#endif