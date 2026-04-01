.syntax  unified
                .arch    armv8-m.main

                .section .vectors
                .align   2
                .globl   __Vectors

__Vectors:
                .long    __StackTop_CPU0     /* CPU0 top of stack */
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

                ldr  r2, =0x5001F000       /* CPU_IDENTITY CPUID register */
                ldr  r2, [r2]
                cbz  r2, 1f

1:
                b    Reset_Handler_CPU0

                .thumb_func
                .globl   Reset_Handler_CPU0
Reset_Handler_CPU0:
                ldr  r0, =__StackTop_CPU0
                msr  msp, r0
                nop
                b    main


HardFault_Handler:
                b        .

                .end
