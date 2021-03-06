.PHONY:
all: clean build/xv6.img

qemu-gdb: clean build/xv6.img
	qemu-system-i386 -nographic -smp 2 -m 512 -drive file=build/xv6.img,index=0,media=disk,format=raw -S -gdb tcp::2510

.PHONY:
clean: 
	rm -rf build && mkdir -p build/kernel

CC = clang-10
CFLAGS = \
	-fno-pic \
	-fno-pie \
	-static \
	-fno-builtin \
	-fno-strict-aliasing \
	-fno-omit-frame-pointer  \
	-fno-stack-protector  \
	-nostdinc \
	-Wall -ggdb -Werror -Wno-unused-function \
	-Iinclude -m32 \
	-std=c18
# -fno-pic 		= disable position-independent code
# -no-pie 		= disable position-independent executable
# -static		= link statically
# -fno-builtin	= don't recognise builtin functions
# -fno-omit-frame-pointer = don't optimise away rbp usage
# -fno-stack-protector = don't include code protecting from buffer overflow
# -nostdinc		= ignore standard include paths
# -Oz			= optimise aggressively for size


build/bootblock: boot/loader.asm boot/loader.c
	nasm -f elf32 boot/loader.asm -o build/loader.o
	$(CC) $(CFLAGS) -Oz -c boot/loader.c -o build/loader_c.o
	ld -m elf_i386 -N -e start -Ttext 0x7C00 build/loader.o build/loader_c.o -o build/bootblock.o
	objcopy -S -O binary -j .text build/bootblock.o build/bootblock
	python3 sign.py build/bootblock

build/kernel_img: kernel/entry.asm kernel/main.c kernel/memory.c kernel/spinlock.c kernel/console.c string.c kernel/mp.c kernel/console.c kernel/lapic.c kernel/proc.c kernel/picirq.c kernel/ioapic.c kernel/uart.c
	nasm -f elf32 -g kernel/entry.asm -o build/kernel/entry.o
	$(CC) $(CFLAGS) -c kernel/main.c -o build/kernel/main.o
	$(CC) $(CFLAGS) -c kernel/memory.c -o build/kernel/memory.o
	$(CC) $(CFLAGS) -c kernel/mp.c -o build/kernel/mp.o
	$(CC) $(CFLAGS) -c kernel/spinlock.c -o build/kernel/spinlock.o
	$(CC) $(CFLAGS) -c kernel/console.c -o build/kernel/console.o
	$(CC) $(CFLAGS) -c string.c -o build/string.o
	$(CC) $(CFLAGS) -c kernel/lapic.c -o build/kernel/lapic.o
	$(CC) $(CFLAGS) -c kernel/proc.c -o build/kernel/proc.o
	$(CC) $(CFLAGS) -c kernel/picirq.c -o build/kernel/picirq.o
	$(CC) $(CFLAGS) -c kernel/ioapic.c -o build/kernel/ioapic.o
	$(CC) $(CFLAGS) -c kernel/uart.c -o build/kernel/uart.o
	ld -m elf_i386 -T kernel/linker_script.ld  build/kernel/uart.o build/kernel/picirq.o build/kernel/ioapic.o build/kernel/proc.o build/kernel/lapic.o build/kernel/mp.o build/kernel/entry.o build/kernel/main.o build/kernel/memory.o build/kernel/spinlock.o build/kernel/console.o  build/string.o -o build/kernel_img

build/xv6.img: build/bootblock build/kernel_img
	dd if=/dev/zero of=build/xv6.img count=10000
	dd if=build/bootblock of=build/xv6.img conv=notrunc
	dd if=build/kernel_img of=build/xv6.img seek=1 conv=notrunc




