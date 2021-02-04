; Protection Enable flag
%define CR0_PE      0x00000001

%define SEG_KERNEL_CODE 1
%define SEG_KERNEL_DATA 2

extern bootmain

bits 16         ; Emit 16-bit code

global start

start:
    cli         ; Disable interrupts

    ; Zero data segment registers DS, ES and SS
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Allow using the 21st bit of address with segmented addressing
seta20.1:
    in al, 0x64      ; wait for not busy
    test al, 0x2
    jnz seta20.1

    mov al, 0xd1     ; 0xd1 -> port 0x64
    out 0x64, al

seta20.2:
    in al, 0x64             ; wait for not busy
    test al, 0x2
    jnz seta20.2

    mov al, 0xdf            ; 0xdf -> port 0x60
    out 0x60, al


    ; Switch from real to protected mode (allows physi-cal addresses to have many more bits).
    ; and ‘32-bit’’ mode (registers, virtual addresses, and most integer arithmetic to be 
    ; carried out with 32 bits rather than 16.

    ; Use a temporary GDT that maps virtual addresses directly to physical addresses
    ; Sets up the segment descriptor table
    lgdt [gdtdesc]
    ; Enable protected mode
    mov eax, cr0
    or eax, CR0_PE      
    mov cr0, eax

    ; Far jump to 32-bit code to reload cs and eip
    jmp (SEG_KERNEL_CODE << 3):start32

bits 32                     ; Emit 32-bit code

start32:
    ; Set up the segment registers to use the GDT below
    mov ax, (SEG_KERNEL_DATA << 3)
    mov ds, ax              ; Make the data segments use the data segment selector
    mov es, ax
    mov ss, ax
    xor ax, ax              ; Zero the extra segments
    mov fs, ax
    mov gs, ax

    ; Set up the stack pointer and call into the C part of the bootloader
    mov esp, start
    call bootmain

spin:
    jmp spin

align 4
gdt:
    dq 0

    ; code segment
    dw 0xffff       ; limit 0:15 (maximum virtual address 4GB)
    dw 0x0          ; base 0:15
    db 0x0          ; base 16:23
    db 0b10011010   ; access flags
    db 0b11001111   ; granularity + limit 16:19
    db 0x0          ; base 24:31

    ; data segment
    dw 0xffff       ; limit 0:15
    dw 0x0          ; base 0:15
    db 0x0          ; base 16:23
    db 0b10010010   ; access flags
    db 0b11001111   ; granularity + limit 16:19
    db 0x0          ; base 24:31

gdtdesc:
    dw (gdtdesc - gdt - 1)  ;sizeof(gdt) - 1
    dd gdt