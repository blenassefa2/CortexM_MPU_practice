
#include <stdint.h>
#include "mpu.h"

extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint8_t user_stack[1024];
extern void setup_cpu1(void);;

#define USER_STACK_TOP ((uint32_t)(user_stack))

int main( void )
{
    /* Exercise 1 - 3 */
    mpu_setup();

    setup_cpu1();

    /* Exercise 4 - Test the MPU */

    // test - 1 
    int x = 0;
    uint32_t *ptr = (uint32_t *) 0x00000001;
    
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
    while(1);   
}

void MemManage_Handler(void) {
    volatile uint32_t mmfsr = *(volatile uint32_t*)0xE000ED28 & 0xFF; // bottom byte = MMFSR
    volatile uint32_t mmfar = *(volatile uint32_t*)0xE000ED34;         // faulting address


    while(1);   
}