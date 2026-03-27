#include "../include/print.h"
#include "../include/shared.h"
#include "../cmsis/include/ARMCM33_DSP_FP.h"              /* the device header file, in this case ARMCM33.h */

extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint8_t user_stack[1024];
#define USER_STACK_TOP ((uint32_t)(user_stack))

extern int counter;
extern void mpu_setup();


static void run_experiment(void)
{
    for (int i = 0; i < ITERATIONS; i++) {
        volatile uint32_t tmp = shared_counter;
        // for (int j = 0; j < 200; j++) ;
        tmp = tmp + 1;
        shared_counter = tmp;
    }
}

void main_cpu1(void)
{
    sh_puts("- cpu #1 main()\n");
    // Without this mpu_setup all the tests below will fail
    // MPU setup is processor level and as long as it is not set up it is considered 
    // mpu_setup();

    // int x = 0;
    // uint32_t *ptr = (uint32_t *) 0x00000000; // 1 => UsageFault (before  MPU fault) 
    
    // x = *ptr;   /* Read test  */
    // sh_puts("- cpu #1 reading passed \n");
    

    // Test 1
    // sh_puts("- cpu #1 testing writing \n");
    // *ptr = 0xA;
    // sh_puts("- cpu #1 read-only region should not be written on, even from priviledged level\n");

    // Test 2
    // sh_puts("- cpu #1 testing unprivileged \n");
    // switch_to_unprivileged(USER_STACK_TOP, user_main);
    // sh_puts("- cpu #1 unprivileged code should not read \n");


    // Test 3 cache to be implemented
    __DSB(); /* Force memory writes before continuing */
    ready_cpu1 = 1;
    __DSB(); /* Force memory writes before continuing */
    while (ready_cpu0 == 0) { ; }

    sh_puts("- cpu1: running\n");
    run_experiment();
   

    // debugging
    __DSB(); /* Force memory writes before continuing */
    done_cpu1 = 1;
    __DSB(); /* Force memory writes before continuing */
    
    while (done_cpu0 == 0) { ; }   // wait for cpu0 to finish
    sh_puts("- cpu1: done\n");
    sh_puts("- expected: ");
    sh_putx(2 * ITERATIONS);
    sh_puts("\n");
    sh_puts("- actual:   ");
    sh_putx(shared_counter);
    sh_puts("\n");
    
    while (1);
}