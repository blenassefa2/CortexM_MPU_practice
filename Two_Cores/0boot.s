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
                .long    MemManage_Handler   /* MemManage */

                .thumb
                .section .text
                .align   2

                .thumb_func
                .globl   Reset_Handler
Reset_Handler:
                ldr  r0, =__Vectors        /* vector table base = 0x00200000 */
                ldr  r1, =0xE000ED08       /* SCB->VTOR */
                str  r0, [r1]              /* VTOR = 0x00200000 */
                ldr  r0, =__StackTop       /* now load stack top separately */
                msr  msp, r0               /* MSP = 0x28208000 */
                nop
                b    main

                .thumb_func
                .weak    HardFault_Handler
HardFault_Handler:
                b        .

                .end
