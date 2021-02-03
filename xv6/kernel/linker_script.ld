OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)


SECTIONS
{
    . = 0x80100000; /* The special symbol name ‘.’ indicates the location counter.  */

    .text : AT(0x100000) { /* Specifies the load address of the section */
        /* Input sections text, stub, gnu will be intermingled, appearing in the same order as they are found in the linker input */
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    PROVIDE(etext = .);

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /* Include debugging information */
    .stab : {
        PROVIDE(__STAB_BEGIN__ = .);ASSERT
        *(.stab);
        PROVIDE(__STAB_END__ = .);
        BYTE(0)
    }

    .stabstr : {
        PROVIDE(__STABSTR_BEGIN__ = .);
        *(.stabstr);
        PROVIDE(__STABSTR_END__ = .);
        BYTE(0)
    }

    /* ALIGN returns the current location counter aligned upward to the specified value.*/
     /* Page-align the data segment */
    . = ALIGN(0x1000);

    PROVIDE(kernel_data = .);

    .data : {
        *(.data)
    }

    PROVIDE(edata = .);

    .bss : {
        *(.bss)
    }

    PROVIDE(kernel_end = .);

     /* Input sections which are assigned to an output section named DISCARD are not included in the output file */
     /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}