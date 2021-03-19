#include "kernel/console.h"
#include "kernel/file.h"
#include "kernel/uart.h"
#include "kernel/traps.h"
#include "kernel/ioapic.h"
#include "kernel/param.h"
#include "string.h"
#include "x86_asm.h"

#define BACKSPACE 0x100
#define CRTPORT 0x3d4      // Index register
#define BLANK_COLOR 0x0700 // determines cursor color on blank screen //
#define INPUT_BUF 128

#define c_with_ctrl(x) ((x) - '@') // Control-x: '@'(64) is the first symbol of printing characters \
                                   // ctrl-D(Non-Printing Character) is 4 in ascii table

DeviceOperations devices[MAX_NUM_DEV]; // TODO: transfer to file.c
struct
{
    char buf[INPUT_BUF];
    uint r; // Read index
    uint w; // Write index
    uint e; // Edit index
} input_buffer;

static int panicked = 0;
// CGA memory,  0x000B8000 - 0x000BFFFF - Color Video Memory
static u16 *crt = (u16 *)P2V(0xb8000);

void panic(const char *func, char *fmt, ...)
{
    // TODO:
    for (;;)
    {
    }
}

void log(const char *func, const char *type, char *fmt, ...)
{
    // TODO:
}

void cga_putc(int c)
{
    // Cursor position: col + 80*row.
    out_u8(CRTPORT, 14);               // 0x0E: Cursor Location High Byte. Tell hardware we are setting the high cursor byte.
    int pos = in_u8(CRTPORT + 1) << 8; // Data register. Send the high cursor byte.
    out_u8(CRTPORT, 15);               // 0x0F: Cursor Location Low Byte
    pos |= in_u8(CRTPORT + 1);

    if (c == '\n')
        pos += 80 - pos % 80;
    else if (c == BACKSPACE && (pos > 0))
    {
        --pos;
    }
    else
        // Each character is 16 bits. In ASCII 8 bits are used to represent a character.
        // The VGA hardware uses 8 bits to designate foreground and background colours (4 bits each).
        // 12-15 Background color and 8-11 Foreground color
        crt[pos++] = (c & 0xff) | BLANK_COLOR; // black on white

    if (pos < 0 || pos > 25 * 80)
        PANIC("Cursor position is under/overflow");

    if ((pos / 80) >= 24)
    { // Scroll up. TODO!
        // memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
        // pos -= 80;
        // memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
    }

    // Change the cursor location: write high and low byte in Data register
    out_u8(CRTPORT, 14);
    out_u8(CRTPORT + 1, pos >> 8);
    out_u8(CRTPORT, 15);
    out_u8(CRTPORT + 1, pos);
    crt[pos] = ' ' | 0x0700;
}

void cons_putc(int c)
{
    if (panicked)
    {
        cli();
        for (;;)
            ;
    }

    if (c == BACKSPACE)
    {
        uart_putc('\b');
        uart_putc(' ');
        uart_putc('\b');
    }
    else
    {
        uart_putc(c);
    }
    cga_putc(c);
}

int console_write(Inode *ip, char *buf, int n)
{
    // iunlock(ip); TODO!
    // acquire(&cons.lock); TODO!!!
    for (int i = 0; i < n; i++)
        cons_putc(buf[i] & 0xff);
    //release(&cons.lock); TODO!!
    // ilock(ip);

    return n;
}

int console_read(Inode *ip, char *dst, int n)
{
    // iunlock(ip);
    u32 target = n;
    // acquire(&cons.lock)
    while (n > 0)
    {
        while (input_buffer.r == input_buffer.w) // nothing to read
        {
            // TODO!!!!!!!!!1
            // if (myproc()->killed)
            // {
            //     release(&cons.lock);
            //     ilock(ip);
            //     return -1;
            // }
            // sleep(&input_buffer.r, &cons.lock);
        }
        int c = input_buffer.buf[input_buffer.r++ % INPUT_BUF];
        // EOF
        if (c == c_with_ctrl('D'))
        {
            if (n < target)
            {
                // Save ^D for next time, to make sure
                // caller gets a 0-byte result.
                input_buffer.r--;
            }
            break;
        }
        *dst++ = c;
        --n;
        if (c == '\n')
            break;
    }

    // release(&cons.lock);
    // ilock(ip);

    return target - n;
}

void console_init(void)
{
    // initlock(&cons.lock, "console"); TODO!!!!

    devices[CONSOLE].write = console_write;
    devices[CONSOLE].read = console_read;
    // cons.locking = 1;

    ioapic_enable(IRQ_KBD, 0);
}