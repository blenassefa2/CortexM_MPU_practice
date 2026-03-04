# CortexM MPU programming experiment

So I am trying to understand MPU configuration and security of embedded IoT devices and this repository shows the learning process and working implementations of configurations, cpu emulation and debugging.

I am trying to follow [Arm's developer tutorial](https://developer.arm.com/documentation/ka005771/latest/) on MPU programming but with open source resources

To use this repository clone it as follows
```bash
git clone --recurse-submodules https://github.com/blenassefa2/CortexM_MPU_practice
```

**Part 1**:  (ARM CortexM M3 CPU on lm3s6965evb machine(board))
- create a very simple firmware
- emulate it on qemu
- debug with gdb
    - have breakpoint on main and make the debugger reach this main without any fault

__issues faced__
- I started this part with Cortex M33 CPU and mps2-an505 machine(board) but qemu didn't understand proper way of emulating this machine
- Instead of setting up the stack pointer properly based on the vector table it kept using the vector table as a kernel code to run and it hardfaults even before beginning anything
- **Trials to solve this issue**:  I thought CortexM33 was maybe too complex so I used CortexM3 Cpu and the simple good board for this CPU was lm3s6965evb so I switched the board as well. But before this decision 
    - I tried changing only the board with other ARM boards or tried to update the way vector table is configured or fault handling was done and none of them were correct
- **Conclusion** The board(machine) is not well supported by qemu

*Folder* [M3](/M3/)

**Part 2**: (ARM CortexM M33 CPU on lm3s6965evb machine(board))
- Do the same as part 1 but with the M33 cpu
- Identify the MPU_TYPE configuration
- Apparently the lm3s6965evb machine does work with M33 CPU so I only had to change the cpu flag on qemu for it to correctly boot
- For identifying MPU registers I need to check the MPU register memory location from ARM since these registers are Memory Mapped registers
    - Hardware registers are placed inside the same address space as RAM — so you access hardware by reading/writing memory addresses.
    - You can't simply see their value using ```info registers``` command on gdb
    - instead you use the examine gdb command
    ```gdb
    x / x  w 0xE000ED90 
    │  │   │		│ 
    │  │   │		└── address to read from 
    │  │   └───── size of each unit 
    │  └──────── format to display in (x implies hexadecimal format)
    └────────────── examine command

    ```

- complete Exercise 1 - Find out how many MPU Regions are present in your Device and confirm that the CMSIS Device Header has value too [Arm's developer tutorial](https://developer.arm.com/documentation/ka005771/latest/)

- for more info on this exercise check comments in [main.c](/M33/main.c)

![debug.png](/M33/docs/image.png)


*Folder* [M33](/M33/)


**Part 3**: (ARM CortexM M33 CPU on mps2-an505 machine(board))

to check if teh mps2-an505 board is supported use the following command

```bash
qemu-system-arm -machine help | grep -v Cortex-M33
```

if mps2-an505 doesn't exist clone the qemu projet itself and set up updated version of qemu-system-arm

```bash
sudo apt update
sudo apt install ninja-build build-essential libglib2.0-dev libpixman-1-dev

git clone https://gitlab.com/qemu-project/qemu.git
cd qemu
./configure --target-list=arm-softmmu
make -j$(nproc)
```

Then from now on you will not be using the ```qemu-system-arm``` command instead you will be using the qemu you configured in your ```\qemu\build``` folder


```bash
# check if the machine exists
./build/qemu-system-arm -machine help | grep an505
```

Therefore make sure to have updated [Makefile](/AN505/Makefile)

issues


```
// The sp and pc need to be properly set up since the mps2-an505 doesn't load as easily as other machines on qemu
// what is its problem?
    // Other machines directly set up the sp from the vector table 
    // but our dear mps2-an505 keeps on trying to execute the vector table WITHOUT SETTING the sp, pc and WITHOUT LOADING the code
    // so the solution that worked
        //1. first remove -kernel option in make file and replace it with -device loader,file=$(TARGET),cpu-num=0
        -device loader, file=$(TARGET) , cpu-num=0
                                   │        │
                                   |        └──   set the PC and SP of CPU core #0
                                   └── loads the target file 
        //2. load the sp and pc registers manually from the debugger as follows (I think cpu-num=0 part didn't work properly so good second measure)

(gdb) set $sp = 0x28208000 
(gdb) set $pc = 0x00200071
(gdb) break main
Note: breakpoints 1 and 2 also set at pc 0x200016.
Breakpoint 3 at 0x200016: file main.c, line 4.
(gdb) info registers
r0             0x0                 0
r1             0x0                 0
r2             0x0                 0
r3             0x0                 0
r4             0x0                 0
r5             0x0                 0
r6             0x0                 0
r7             0x0                 0
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0x0                 0
sp             0x28208000          0x28208000 <---- now properly setup
lr             0xffffffff          -1
pc             0x200070            0x200070 <Reset_Handler> <---- now properly setup
xpsr           0x41000000          1090519040 
fpscr          0x0                 0
msp            0x28208000          673218560
psp            0x0                 0
msp_ns         0x0                 0
psp_ns         0x0                 0
--Type <RET> for more, q to quit, c to continue without paging--q
Quit
```
