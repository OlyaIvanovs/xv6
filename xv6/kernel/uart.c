// Intel 8250 serial port (UART).block diagram
#include "x86_asm.h"
#include "kernel/traps.h"
#include "kernel/lapic.h"
#include "kernel/ioapic.h"

#define COM1 0x3f8 // Base Port I/O address

static int uart_ready = 0; // is there a uart?

void uart_putc(char c)
{
    if (!uart_ready)
    {
        return;
    }
    for (int i = 0; i < 128 && !(in_u8(COM1 + 5) & 0x20); i++) // Bit 6(Line Status Register) is set to a logical "1" if all characters have been transmitted
        microdelay(10);
    out_u8(COM1 + 0, c); // write to Transmitter Holding Buffer
}

void uart_init()
{
    out_u8(COM1 + 2, 0);    // Disable the FIFOs
    out_u8(COM1 + 3, 0x80); // Line Control Register: turn on 7 bit(Divisor Latch Access Bit):  when it is set to "1", the baud rate registers can be set
    out_u8(COM1 + 0, 0x0c); // Divisor Latch Low Byte: 115200 / 9600 (baud rate) Speed of transition
    out_u8(COM1 + 1, 0);    // Divisor Latch High Byte
    out_u8(COM1 + 3, 0x03); // Lock divisor and  Bit 0 and Bit 1 control how many data bits are sent for each data "word" that is transmitted via serial protocol(11: 8 bits).
    out_u8(COM1 + 4, 0);
    out_u8(COM1 + 1, 0x01); // Interrrupt Enable Register: interrupts (3: Modem Status, 2: Receiver Line Status, 1: Transmitter Holding Register Empty, 0: Received Data Available)

    // If status is 0xFF, no serial port.
    if (in_u8(COM1 + 5) == 0xFF) // Line status register
        return;
    uart_ready = 1;

    // Acknowledge pre-existing interrupt conditions;
    in_u8(COM1 + 2);            //Interrupt Identification Register
    in_u8(COM1 + 0);            // Receiver Buffer
    ioapic_enable(IRQ_COM1, 0); // Enable interrupts

    // Announce that we're here.
    for (char *p = "xv6...\n"; *p; p++)
        uart_putc(*p);
}
