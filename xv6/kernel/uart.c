// Intel 8250 serial port (UART).block diagram
#include "x86_asm.h"
#include "traps.h"

#define COM1 0x3f8 // Base Port I/O address

static int uart_ready = 0; // is there a uart?

void uartputc(char *c)
{
}

void uart_init()
{
    outb(COM1 + 2, 0);    // Disable the FIFOs
    outb(COM1 + 3, 0x80); // Line Control Register: turn on 7 bit(Divisor Latch Access Bit):  when it is set to "1", the baud rate registers can be set
    outb(COM1 + 0, 0x0c); // Divisor Latch Low Byte: 115200 / 9600 (baud rate)
    outb(COM1 + 1, 0);    // Divisor Latch High Byte
    outb(COM1 + 3, 0x03); // Lock divisor and  Bit 0 and Bit 1 control how many data bits are sent for each data "word" that is transmitted via serial protocol(11: 8 bits).
    outb(COM1 + 4, 0);
    outb(COM1 + 1, 0x01); // Interrrupt Enable Register: interrupts (3: Modem Status, 2: Receiver Line Status, 1: Transmitter Holding Register Empty, 0: Received Data Available)

    // If status is 0xFF, no serial port.
    if (inb(COM1 + 5) == 0xFF) // Line status register
        return;
    uart_ready = 1;

    // Acknowledge pre-existing interrupt conditions;
    inb(COM1 + 2);              //Interrupt Identification Register
    inb(COM1 + 0);              // Receiver Buffer
    ioapic_enable(IRQ_COM1, 0); // Enable interrupts

    // Announce that we're here.
    for (char *p = "xv6...\n"; *p; p++)
        uartputc(*p);
}
