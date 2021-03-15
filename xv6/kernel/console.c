#include "kernel/console.h"
#include "kernel/file.h"

#define BACKSPACE 0x100

static int panicked = 0;

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

void consputc(int c)
{
    if (panicked)
    {
        cli();
        for (;;)
            ;
    }

    if (c == BACKSPACE)
    {
        uartputc('\b');
        uartputc('  ');
        uartputc('\b');
    }
    else
    {
        uartputc(c);
    }
    cgaputc(c);
}

int console_write(Inode *ip, char *buf, int n)
{
    // iunlock(ip); TODO!
    // acquire(&cons.lock); TODO!!!
    for (int i = 0; i < n; i++)
        consputc(buf[i] & 0xff);
    //release(&cons.lock); TODO!!
    // ilock(ip);

    return n;
}

void console_init(void)
{
    // initlock(&cons.lock, "console"); TODO!!!!

    devsw[CONSOLE].write = consolewrite;
    devsw[CONSOLE].read = consoleread;
    cons.locking = 1;
}