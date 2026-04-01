  .syntax unified
    .cpu cortex-m33
    .thumb

    .global switch_to_unprivileged
    .type switch_to_unprivileged, %function
    .extern user_main

// switch_to_unprivileged(USER_STACK_TOP, user_main)
    .thumb_func
switch_to_unprivileged:
    // r0 = address of user stack top (caller must pass)
    // r1 = address of user_main (caller must pass)
    mov r2, r0
    msr psp, r2

    mov r3, #3
    msr CONTROL, r3
    isb

    bx r1

    // all of this below doesn't work (Trial for multicore)
    @ .syntax unified
    @ .cpu cortex-m33
    @ .thumb

    @ .global startup_cpu1
    @ .type startup_cpu1, %function
    @ .thumb_func
    
    @ .equ SYSCTL_BASE,   0x50021000
    @ .equ CPUWAIT_OFF,   0x118       // bit1 = CPU1, bit0 = CPU0
