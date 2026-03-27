#include "../../../include/fault/fault_handlers.h"
#include "../../../include/print.h"
#include "../../../include/multicore/cpu_control.h"

// SCB registers
// #define HFSR   (*(volatile uint32_t*)0xE000ED2C)
// #define MMFSR  (*(volatile uint32_t*)0xE000ED28)
// #define MMFAR  (*(volatile uint32_t*)0xE000ED34)


// -------- Handlers --------

void HardFault_Handler(void) {
    volatile uint32_t hfsr  = *(volatile uint32_t*)0xE000ED2C; // HardFault Status
    volatile uint32_t mmfar = *(volatile uint32_t*)0xE000ED34; // MemManage Fault Address
    sh_puts(get_cpu_id()==0?"CPU#0":"CPU#1");
    sh_puts(" ");
    sh_puts("HardFault at: ");
    sh_putx(mmfar);
    sh_puts(" status: ");
    sh_putx(hfsr);
    sh_puts("\n");
    while(1);   
}

void MemManage_Handler(void) {
    volatile uint32_t mmfsr = *(volatile uint32_t*)0xE000ED28 & 0xFF; // bottom byte = MMFSR
    volatile uint32_t mmfar = *(volatile uint32_t*)0xE000ED34;         // faulting address
    sh_puts(get_cpu_id()==0?"CPU#0":"CPU#1");
    sh_puts(" ");
    sh_puts("MemManageFault at: ");
    sh_putx(mmfar);
    sh_puts(" status: ");
    sh_putx(mmfsr);
    sh_puts("\n");
    while(1);   
}