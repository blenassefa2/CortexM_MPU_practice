# Cortex-M MPU & Side-Channel Experiments

This repository explores memory protection and side channels on ARM Cortex-M systems using bare-metal firmware and emulation.

## Goals

- Understand **Memory Protection Unit (MPU)** behavior on Cortex-M
- Explore **multi-core interactions** (Cortex-M33 dual-core)
- Investigate **timing side channels** using hardware features (e.g., DWT, SysTick)
- Build reproducible experiments using open-source tooling


## 🚀 Getting Started

Clone the repository:

```bash
git clone --recurse-submodules https://github.com/blenassefa2/CortexM_MPU_practice
cd CortexM_MPU_practice
```

Build and run an example:

```bash
cd experiments/multicore
make run
```

##  Key Topics

- MPU configuration and region isolation
- TrustZone basics (secure vs non-secure execution)
- Dual-core synchronization and shared memory
- Timing measurements using:
  - SysTick
  - DWT cycle counter
- Side-channel primitives:
  - Shared memory contention
  - Cross-core timing leakage


## Tooling

- QEMU (ARM Cortex-M emulation)
- GDB (low-level debugging)
- CMSIS (ARM device headers)
- Custom bare-metal runtime


## Repository Structure

```
.
├── Readme.md  
├── setup/                  # QEMU setup and board experiments (Part 1: Understanding Microcontrollers)
|
└── Two_Cores/              # Mostly two core configurations with multiple side channel attack experiments (Part 2: MCU setup and Side channel attack implementations)
    |
    ├── python/             
    │   ├── plot_timing.py
    │   └── requirements.txt
    |
    ├──  README.md
    ├──  Makefile           # Includes targets for each experiment
    ├──  link.ld            # default linker script when not specified in experiment_*/ folders
    ├── include/
    │   ├── cmsis/          # some arm libraries
    │   ├── fault/          # hardfault and memfault handling algorithms
    │   ├── multicore/      # multicore initiation logics
    │   ├── platform/       # specification of boards or cores ... (Raspbpico, an521, cortexm33, cortexm0+)
    │   ├── timing/         # different timing options (for example: systick , dwt)
    │   ├── mpu/            # different mpu set up for different architectures (for example: armv6, armv8)
    │   ├── print.h
    │   └── shared.h        # global variables for testing core concurrency ...
    │
    └── src/
        ├──  common/        # implementations of the included libraries
        |    ├── fault/
        |    ├── multicore/
        |    ├── platform/
        |    └── timing/
        ├── experiment_1/
        |   ├── Notes.md
        |   ├── 0boot.s
        │   ├── main.c
        │   ├── user.c
        │   └── user_caller.s
        │
        ├── experiment_2/
        |   ├── Notes.md
        |   ├── link.ld     # Custom linker script
        |   ├── 0boot.s
        │   ├── cpu0.c
        │   └── cpu1.c      # Notes, diagrams, and explanations
        ...
        └── experiment_n/




```

## Example Experiments

[MPU Basics](Setups/README.md)
- Detect number of MPU regions
- Configure memory regions
- Trigger and debug faults

Dual-Core Execution
- Boot secondary core
- Synchronize cores using shared memory

Side Channels
- Measure timing differences across cores
- Explore cache and memory contention effects
- Investigate MPU-related timing leakage


# References
- ARM MPU Programming Guide (to be linked)
- CMSIS Documentation (to be linked)
- QEMU ARM System Emulator (to be linked)
- Research Paper1  (to be linked)
- Research Paper2 (to be linked)
- Research Paper3 (to be linked)
- Research Paper4 (to be linked)