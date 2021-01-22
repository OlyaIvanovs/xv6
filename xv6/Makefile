qemu-gdb: build/xv6.img
	qemu-system-i386 -nographic -smp 2 -m 512 -drive file=build/xv6.img,index=0,media=disk,format=raw -S -gdb tcp::251000

.PHONY:
clean: 
	rm -rf build && mkdir build/kernel

bootblock: boot/loader.asm boot/loader.c
	nasm -f elf32 boot/loader.asm -o build/loader.o
	clang -c -ggdb -m32 boot/loader.c -o build/loader_c.o
	ld -m elf_i386 -N -e start -Ttext 0x7C00 build/loader.o build/loader_c.o -o build/bootblock.o
	objcopy -S -O binary -j .text build/bootblock.o build/bootblock
	python3 sign.py build/bootblock

xv6.img: build/bootblock
	dd if=/dev/zero of=build/xv6.img count=10000
	dd if=build/bootblock of=build/xv6.img conv=notrunc
