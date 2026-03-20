

#include "../include/mpu.h"
#include "../include/print.h"

extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint32_t __Vectors;

extern uint8_t user_stack[1024];

#define SYSCTL_BASE       0x50021000UL
#define INITSVTOR1_OFF    0x114UL
#define CPUWAIT_OFF       0x118UL
#define CPU_IDENTITY_BASE 0x5001F000UL



static uint32_t get_cpu_id(void)
{
    return *(volatile uint32_t *)CPU_IDENTITY_BASE;
}

static void setup_cpu1(void)
{
    volatile uint32_t *initsvtor1 = (volatile uint32_t *)(SYSCTL_BASE + INITSVTOR1_OFF);
    volatile uint32_t *cpuwait = (volatile uint32_t *)(SYSCTL_BASE + CPUWAIT_OFF);

    *initsvtor1 = (uint32_t)&__Vectors;
    *cpuwait &= ~2u;
}

#define USER_STACK_TOP ((uint32_t)(user_stack))


void main_cpu0(void)
{
    sh_puts("- mpu setup()\n");
    /* Exercise 1 - 3 */
    mpu_setup();

    sh_puts("- cpu #1 setup()\n");
    setup_cpu1();

    sh_puts("- cpu #0 main()\n");

    /* Exercise 4 - Test the MPU */
    sh_puts("- cpu #0 sleep()\n");
    for(int i=0;i<10000000;i++) ; 

    sh_puts("- cpu #0 test MPU \n");
    // test - 1 
    int x = 0;
    uint32_t *ptr = (uint32_t *) 0x00000000; // 1 => UsageFault (before  MPU fault) 
    
    x = *ptr;   /* Read test  */
    // *ptr = 0xA;  /* Write test: ptr is in region 0 which has read only  access */
    
    // test - 2
    // switch_to_unprivileged(USER_STACK_TOP, user_main);


    
    /* Exercise 5 - Test with TT instruction */
    // Currently it doesn't work because I am in Nonsecure priviledged mode
    // it only works properly if TrustZone Secure mode is enabled
    // volatile uint32_t raw;
    // asm volatile (
     
    //     "TT %1, %0\n\t"
    //     : "=r" (raw)
    //     : "r" (ptr)
    //     : 
    // );

    // struct address_info_t *test_result = (struct address_info_t *)&raw;

    while (1);
}



// Read fault status registers and loop forever
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
