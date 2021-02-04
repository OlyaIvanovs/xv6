%define KERNBASE    0x80000000
; Size of per-process kernel stack
%define KERN_STACK_SIZE 4096

; Page size extension (enables 4Mb pages)
%define CR4_PSE     0x00000010
; Paging
%define CR0_PG      0x80000000
; Write protect
%define CR0_WP      0x00010000


; By convention, the _start symbol specifies the ELF entry point.
; Since we haven't set up virtual memory yet, our entry point is
; the physical address of 'entry'.
global _start
_start equ (entry - KERNBASE)
extern kernel_start
extern entry_page_dir

bits 32

section .text

; The boot processor jumps here after executing the boot loader
entry:
    ; The kernel tells the paging hardware to allow super pages(4Mbyte) 
    ; by setting the CR_PSE bit in cr4.
    mov eax, cr4
    or eax, CR4_PSE
    mov cr4, eax

    ; Set page directory. Load the physical address of entrypgdir into control register cr3.
    mov eax, (entry_page_dir - KERNBASE)
    mov cr3, eax

    ; To enable the paging hardware, xv6 sets the flag CR0_PG in the control register cr0.
    mov eax, cr0
    or eax, (CR0_PG | CR0_WP)
    mov cr0, eax

    ; Set up the stack pointer; 
    mov esp, stack + KERN_STACK_SIZE

    ; Jump to kernel_start and switch to high addresses
    mov eax, kernel_start
    jmp eax

section .bss
stack:      resb KERN_STACK_SIZE