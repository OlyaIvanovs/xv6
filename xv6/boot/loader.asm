bits 16

global start

start:
    cli

    ; Zero data segment registers DS, ES and SS
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Allow using the 21st bit of address with segmented addressing
seta20.1:
    in al, 0x64
    test al, 0x2
    jnz seta20.1


    mov al, 0xd1
    out 0x64, al

seta20.2:
    in al, 0x64
    test al, 0x2
    jnz seta20.2

    mov al, 0xdf
    out 0x60, al


    ; Switch from real to protected mode.
    lgdt [gdtdesc]

spin:
    jmp spin

align 4
gdt:
    dq 0

    ; code segment
    dw 0xffff       ; limit 0:15
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