#include "../../../include/multicore/cpu_control.h"
#include "../../../include/platform/an521.h"





uint32_t get_cpu_id(void)
{
    return *(volatile uint32_t *)CPU_IDENTITY_BASE;
}

void setup_cpu1()
{
    volatile uint32_t *initsvtor1 = (volatile uint32_t *)(SYSCTL_BASE + INITSVTOR1_OFF);
    volatile uint32_t *cpuwait = (volatile uint32_t *)(SYSCTL_BASE + CPUWAIT_OFF);

    *initsvtor1 = (uint32_t)&__Vectors;
    *cpuwait &= ~2u;
}