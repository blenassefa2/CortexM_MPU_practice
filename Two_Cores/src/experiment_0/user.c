#include <stdint.h>

uint8_t user_stack[1024];



void user_main(void) {
    volatile uint32_t val;
    volatile uint32_t *ptr = (uint32_t *)0x00000000; 
    
    // Test 2 unauthorized access
    val = *ptr; //reading privileged only read only memory as unprivileged program
    
    // Test 3 buffer-overflow


    
    while (1);
}