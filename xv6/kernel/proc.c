#include "kernel/proc.h"
#include "console.h"
#include "kernel/param.h"
#include "x86_asm.h"

extern CPU gCPUs[MAX_NUM_CPUS]; // defined in kernel/mp.c

int cpuid()
{
    return mycpu() - gCPUs;
}

CPU *mycpu(void)
{
    // The IF flag in the EFLAGS register permits all maskable hardware interrupts to be masked as a group
    if (readeflags() & FL_IF)
        PANIC("mycpu called with interrupts enabled\n");

    int apic_id, i;
}