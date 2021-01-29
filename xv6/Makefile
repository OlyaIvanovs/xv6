.PHONY:
all: clean build/xv6.img

qemu-gdb: build/xv6.img
	qemu-system-i386 -nographic -smp 2 -m 512 -drive file=build/xv6.img,index=0,media=disk,format=raw -S -gdb tcp::251000

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

build/xv6.img: build/bootblock
	dd if=/dev/zero of=build/xv6.img count=10000
	dd if=build/bootblock of=build/xv6.img conv=notrunc
