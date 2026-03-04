#include "ARMCM33_DSP_FP.h"

int main(void) {
    unsigned int num_regions = (MPU->TYPE >> 8) & 0xFF;

    // After this line is executed on gdb print result to see result value 
    // then compare it  with 
    // print (*(unsigned int*)0xE000ED90 >> 8) & 0xFF
    // 0xE000ED90 is the address of MPU_TYPE register
    //      (Remember this register is memory type so you infer it through its memory address 
    //      and you can't examine it with info registers)
    volatile unsigned int result = num_regions;

    while (1);
}