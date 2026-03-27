#ifndef AN521_H
#define AN521_H


// for multicore
#define SYSCTL_BASE       0x50021000UL
#define INITSVTOR1_OFF    0x114UL
#define CPUWAIT_OFF       0x118UL
#define CPU_IDENTITY_BASE 0x5001F000UL


// for systic
#define SYSTICK_CTRL  (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_LOAD  (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_VAL   (*((volatile uint32_t *)0xE000E018))

#endif