#include "../include/print.h"

extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint8_t user_stack[1024];
#define USER_STACK_TOP ((uint32_t)(user_stack))

extern void mpu_setup();
void main_cpu1(void)
{
    sh_puts("- cpu #1 main()\n");
    // Without this mpu_setup all the tests below will fail
    // MPU setup is processor level and as long as it is not set up it is considered 
    mpu_setup();

    int x = 0;
    uint32_t *ptr = (uint32_t *) 0x00000000; // 1 => UsageFault (before  MPU fault) 
    
    x = *ptr;   /* Read test  */
    sh_puts("- cpu #1 reading passed \n");
    

    // Test 1
    sh_puts("- cpu #1 testing writing \n");
    // *ptr = 0xA;
    sh_puts("- cpu #1 read-only region should not be written on, even from priviledged level\n");

    // Test 2
    sh_puts("- cpu #1 testing unprivileged \n");
    switch_to_unprivileged(USER_STACK_TOP, user_main);
    sh_puts("- cpu #1 unprivileged code should not read \n");


    // Test 3
    while (1);
}