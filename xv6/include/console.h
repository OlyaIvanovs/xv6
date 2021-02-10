#ifndef XV6_CONSOLE_H
#define XV6_CONSOLE_H

void panic(const char *func, char *fmt, ...);

#define PANIC(args...) panic(__func__, args)

#endif // XV6_CONSOLE_H