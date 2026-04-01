

#include "../../include/mpu.h"

#include <stdint.h>


extern void switch_to_unprivileged(uint32_t top, void (*user_func)(void));
extern void user_main(void);

extern uint8_t user_stack[1024];


#define USER_STACK_TOP ((uint32_t)(user_stack))

int main( void )
{
    /* Exercise 1 - 3 */
    mpu_setup();


    /* Exercise 4 - Test the MPU */

    // test - 1 
    int x = 0;
    uint32_t *ptr = (uint32_t *) 0x00000001;
    
    x = *ptr;   /* Read test  */
   // *ptr = 0xA;  /* Write test: ptr is in region 0 which has read only  access */
    
    // test - 2
    switch_to_unprivileged(USER_STACK_TOP, user_main);


    
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




