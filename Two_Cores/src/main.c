

#include "../include/mpu.h"
#include "../include/print.h"
#include "../include/shared.h"
#include "../cmsis/include/ARMCM33_DSP_FP.h"              /* the device header file, in this case ARMCM33.h */

extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint8_t user_stack[1024];

#define USER_STACK_TOP ((uint32_t)(user_stack))

extern uint32_t __Vectors;

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

static void run_experiment(void)
{
    for (int i = 0; i < ITERATIONS; i++) {
        volatile uint32_t tmp = shared_counter;
        // for (int j = 0; j < 200; j++) ;
        tmp = tmp + 1;
        shared_counter = tmp;
    }
}

void main_cpu0(void)
{
    sh_puts("- mpu setup()\n");
    /* Exercise 1 - 3 */
    // mpu_setup();
    

    sh_puts("- cpu #1 setup()\n");
    setup_cpu1();

    sh_puts("- cpu #0 main()\n");

    /* Exercise 4 - Test the MPU */
    sh_puts("- cpu #0 sleep()\n");
    for(int i=0;i<10000000;i++) ; 

    sh_puts("- cpu #0 test  \n");
    

    // test - 1 
    // int x = 0;
    // uint32_t *ptr = (uint32_t *) 0x00000000; // 1 => UsageFault (before  MPU fault) 
    
    // x = *ptr;   /* Read test  */
    // *ptr = 0xA;  /* Write test: ptr is in region 0 which has read only  access */
    
    // test - 2
    // switch_to_unprivileged(USER_STACK_TOP, user_main);
   
    // test 4 -  cache race condition
    __DSB(); /* Force memory writes before continuing */
    ready_cpu0 = 1; 
    __DSB(); /* Force memory writes before continuing */
    while (ready_cpu1 == 0) { ; }

    sh_puts("- cpu0: running\n");
    run_experiment();

    __DSB(); /* Force memory writes before continuing */
    done_cpu0 = 1;
    __DSB(); /* Force memory writes before continuing */
    
    while (done_cpu1 == 0) { ; }   // wait for cpu1 to finish
    sh_puts("- cpu0: done\n");
 
   
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
