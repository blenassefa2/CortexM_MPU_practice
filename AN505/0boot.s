.syntax  unified
                .arch    armv8-m.main

                .section .vectors
                .align   2
                .globl   __Vectors

__Vectors:
                .long    __StackTop          /* Top of Stack */
                .long    Reset_Handler       /* Reset Handler */
                .long    0                   /* NMI */
                .long    HardFault_Handler   /* HardFault */

                .thumb
                .section .text
                .align   2

                .thumb_func
                .globl   Reset_Handler
Reset_Handler:
                nop
                b        main

                .thumb_func
                .weak    HardFault_Handler
HardFault_Handler:
                b        .

                .end
