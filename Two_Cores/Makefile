


SRCS = $(wildcard src/*.c) $(wildcard src/*.s)
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
#QEMU = ./qemu/build/qemu-system-arm
QEMU = qemu-system-arm

# toolchain
CC      = arm-none-eabi-gcc
GDB     = gdb-multiarch
MACHINE = mps2-an521 

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
-I./cmsis/include \
-I./include

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
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native -device loader,file=$(TARGET),cpu-num=0 

qemu_debug: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native -device loader,file=$(TARGET),cpu-num=0  -S -s

qemu2cpu: $(TARGET)
	$(QEMU) -machine mps2-an521 -nographic \
	-semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0

#$(QEMU)  -M help | grep -i "lpc\|nxp\|dual" to get boards that support multiple core
load_cpu0:
	$(QEMU) -machine mps2-an521 -nographic \
    -device loader,file=$(TARGET),cpu-num=0 \
    -S -s
	
load_cpu1:
	$(QEMU) -machine mps2-an521 -nographic \
	-device loader,file=$(TARGET),cpu-num=1 \
	-S -gdb tcp::1235

load_both:
	$(QEMU) -machine mps2-an521 -nographic \
	-device loader,file=firmware.elf,cpu-num=0 \
	-device loader,file=firmware.elf,cpu-num=1 \
	-S -s

gdb:
	$(GDB) $(TARGET) -ex "target remote:1234"

gdb1:
	$(GDB) $(TARGET) -ex "target remote:1235"


clean:
	rm -f src/*.o *.o *.elf
