#include "base.h"
#include "kernel/memory.h"

static u8
in_u8(u16 port)
{
    u16 data;
    __asm__ volatile("in %[port], %[data]"
                     : [ data ] "=a"(data)  // output: %eax, %ax, %al (a)
                     : [ port ] "d"(port)); // input: %edx, %dx, %dl (d)
    return data;
}

static void
in_u32_array(u16 port, void *addr, int count)
{
    __asm__ volatile("cld; rep insl"
                     : "=D"(addr), "=c"(count)          // registers  %edi (D), %ecx (c)
                     : "d"(port), "0"(addr), "1"(count) // %edx (d)
                     : "memory", "cc");                 // the code is changing the contents of memory.
}

static void
out_u8(u16 port, u8 data)
{
    __asm__ volatile("out %[data], %[port]"
                     :
                     : [ data ] "a"(data), [ port ] "d"(port)); // registers   %eax, %ax, %al (a), %edx, %dx, %dl (d)
}

// Fills memory byte by byte
static void
store_u8s(void *addr, u8 data, int count)
{
    __asm__ volatile("cld; rep stosb"
                     : "=D"(addr), "=c"(count)
                     : "0"(addr), "1"(count), "a"(data)
                     : "memory", "cc"); // the code is changing the contents of memory.
}

// Fills memory by dwords
static void
store_u32s(void *addr, u32 data, int count)
{
    __asm__ volatile("cld; rep stosl"
                     : "=D"(addr), "=c"(count)
                     : "0"(addr), "1"(count), "a"(data) //  %edi(D) and %ecx(c) are used as both the input and the output variable
                     : "memory", "cc");                 // the code is changing the contents of memory.
}

static void
load_cr3(u32 addr)
{
    __asm__ volatile("movl %[page_table], %%cr3"
                     :
                     : [ page_table ] "r"(addr)); // get stored in General Purpose Registers(GPR)
}

static inline u32
readeflags(void)
{
    u32 eflags;
    // The PUSHF instruction stores all flags on the stack
    __asm__ volatile("pushfl; popl %0"
                     : "=r"(eflags)); // output
    return eflags;
}

static inline void load_gdt(SegDescriptor *p, int size)
{
    volatile u16 pd[3];

    pd[0] = size - 1;
    pd[1] = (u32)p;
    pd[2] = (u32)p >> 16;

    __asm__ volatile("lgdt (%0)" // Load m into GDTR
                     :
                     : "r"(pd));
}
