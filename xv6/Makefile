# =========================================EMULATION========================================================

qemu-gdb: xv6.img
	qemu-system-i386 -nographic -smp 2 -m 512 -drive file=xv6.img,index=0,media=disk,format=raw -S -gdb tcp::251000

bootblock: boot/loader.asm sign.py
	nasm -f elf32 boot/loader.asm -o loader.o
	ld -m elf_i386 -N -e start -Ttext 0x7C00 loader.o -o bootblock.o
	objcopy -S -O binary -j .text bootblock.o bootblock
	python3 	sign.py bootblock


xv6.img: bootblock
	dd if=/dev/zero of=xv6.img count=10000
	dd if=bootblock of=xv6.img conv=notrunc
