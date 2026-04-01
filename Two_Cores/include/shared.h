#ifndef SHARED_H
#define SHARED_H


#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>


#define ITERATIONS 2000000 
/* expected 4 000 000 =  0x3D0900 but kept getting 1EB4A0 when not using atomic */ 


extern uint32_t shared_counter;

extern volatile atomic_bool cpu1_running;
extern volatile atomic_bool cpu0_running;

#endif