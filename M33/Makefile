CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m33 -mthumb -g -O0 -nostdlib -nostartfiles -mfloat-abi=soft -I./cmsis/include
TARGET  = firmware.elf

all: $(TARGET)

$(TARGET): main.c startup.c link.ld
	$(CC) $(CFLAGS) -T link.ld main.c startup.c -o $(TARGET)

qemu: $(TARGET)
	qemu-system-arm -machine lm3s6965evb -cpu cortex-m33 -nographic \
	  -kernel $(TARGET) -S -s

gdb:
	gdb-multiarch $(TARGET)

clean:
	rm -f $(TARGET)