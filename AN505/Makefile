


SRCS = main.c system_ARMCM33.c 0boot.s
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
QEMU = ./qemu/build/qemu-system-arm

# toolchain
CC      = arm-none-eabi-gcc
GDB     = gdb-multiarch
MACHINE = mps2-an505

# flags
CFLAGS  = \
-mcpu=cortex-m33 \
-mthumb -g -O0 \
-nostdlib \
-fno-exceptions \
-fno-unwind-tables \
-fno-asynchronous-unwind-tables \
-nostartfiles \
-mfloat-abi=soft \
-I./cmsis/include

# CFLAGS = -ffreestanding
LDFLAGS = -T link.ld

# final output
TARGET  = firmware.elf


all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

qemu: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic  -device loader,file=$(TARGET),cpu-num=0  -S -s

gdb:
	$(GDB) $(TARGET) -ex "target remote:1234"

clean:
	rm -f *.o *.elf