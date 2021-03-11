#include "x86_asm.h"

// If you are going to use the processor local APIC and the IOAPIC, you must first disable the PIC
// I/O Addresses of the two programmable interrupt controllers
#define IO_PIC1 0x20 // Master (IRQs 0-7)
#define IO_PIC2 0xA0 // Slave (IRQs 8-15)

// Don't use the 8259A interrupt controllers.  Xv6 assumes SMP hardware.
void pic_init(void)
{
    // mask all interrupts
    out_u8(IO_PIC1 + 1, 0xFF);
    out_u8(IO_PIC2 + 1, 0xFF);
}