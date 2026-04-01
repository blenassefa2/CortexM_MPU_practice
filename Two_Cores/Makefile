
EXPERIMENT ?= experiment1

# Toolchain
CC      = arm-none-eabi-gcc
GDB     = gdb-multiarch
QEMU    = qemu-system-arm
MACHINE = mps2-an521


# Directories
INCDIRS := -I./include -I./include/cmsis -I./include/fault -I./include/multicore \
           -I./include/platform -I./include/timing -I./include/mpu

# Compiler flags
CFLAGS  = -mcpu=cortex-m33 -mthumb -g -O0 -nostdlib \
          -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables \
          -nostartfiles -mfloat-abi=soft $(INCDIRS)

# Linker
LDFLAGS = -T link.ld
TARGET  = firmware.elf


# Source files (all common + experiments recursively)
# Common sources
COMMON_SRCS := $(wildcard src/common/**/*.c) $(wildcard src/common/*.c) $(wildcard src/common/**/*.s)

# Experiment-specific sources
EXP_SRCS := $(wildcard src/$(EXPERIMENT)/*.c) $(wildcard src/$(EXPERIMENT)/*.s)

SRCS := $(COMMON_SRCS) $(EXP_SRCS)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# Build rules
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# QEMU Targets
qemu: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0

qemu_debug: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0 -S -s

qemu2cpu: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic \
	-semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0 \
	-device loader,file=$(TARGET),cpu-num=1

# Load individual CPUs
load_cpu0:
	$(QEMU) -machine $(MACHINE) -nographic \
	-device loader,file=$(TARGET),cpu-num=0 -S -s

load_cpu1:
	$(QEMU) -machine $(MACHINE) -nographic \
	-device loader,file=$(TARGET),cpu-num=1 -S -gdb tcp::1235

# GDB targets
gdb:
	$(GDB) $(TARGET) -ex "target remote:1234"

gdb1:
	$(GDB) $(TARGET) -ex "target remote:1235"

# Clean
clean:
	rm -f src/common$(OBJS) $(wildcard src/experiment*/*.o) $(TARGET)

.PHONY: all clean qemu qemu_debug qemu2cpu load_cpu0 load_cpu1 gdb gdb1

# SRCS := $(shell find src -name '*.c' -o -name '*.s')
# OBJS = $(SRCS:.c=.o)
# OBJS := $(OBJS:.s=.o)



# # toolchain
# CC      = arm-none-eabi-gcc
# GDB     = gdb-multiarch
# MACHINE = mps2-an521 
# #QEMU = ./qemu/build/qemu-system-arm
# QEMU = qemu-system-arm

# # flags
# CFLAGS  = \
# -mcpu=cortex-m33 \
# -mthumb -g -O0 \
# -nostdlib \
# -fno-exceptions \
# -fno-unwind-tables \
# -fno-asynchronous-unwind-tables \
# -nostartfiles \
# -mfloat-abi=soft \
# -I./include \
# -I./include/cmsis \
# -I./include/fault \
# -I./include/multicore \
# -I./include/platform \
# -I./include/timing



# # CFLAGS = -ffreestanding
# LDFLAGS = -T link.ld

# # final output
# TARGET  = firmware.elf


# all: $(TARGET)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# %.o: %.s
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# qemu: $(TARGET)
# 	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native -device loader,file=$(TARGET),cpu-num=0 

# qemu_debug: $(TARGET)
# 	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native -device loader,file=$(TARGET),cpu-num=0  -S -s

# qemu2cpu: $(TARGET)
# 	$(QEMU) -machine mps2-an521 -nographic \
# 	-semihosting-config enable=on,target=native \
# 	-device loader,file=$(TARGET),cpu-num=0

# #$(QEMU)  -M help | grep -i "lpc\|nxp\|dual" to get boards that support multiple core
# load_cpu0:
# 	$(QEMU) -machine mps2-an521 -nographic \
#     -device loader,file=$(TARGET),cpu-num=0 \
#     -S -s
	
# load_cpu1:
# 	$(QEMU) -machine mps2-an521 -nographic \
# 	-device loader,file=$(TARGET),cpu-num=1 \
# 	-S -gdb tcp::1235

# load_both:
# 	$(QEMU) -machine mps2-an521 -nographic \
# 	-device loader,file=firmware.elf,cpu-num=0 \
# 	-device loader,file=firmware.elf,cpu-num=1 \
# 	-S -s

# gdb:
# 	$(GDB) $(TARGET) -ex "target remote:1234"

# gdb1:
# 	$(GDB) $(TARGET) -ex "target remote:1235"


# clean:
# 	rm -f src/*.o *.o *.elf
