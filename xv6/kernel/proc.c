#include "kernel/proc.h"
#include "console.h"
#include "kernel/param.h"
#include "x86_asm.h"
#include "kernel/lapic.h"

extern CPU gCPUs[MAX_NUM_CPUS]; // defined in kernel/mp.c
extern int gNumCPUs;            // define in kernel/mp.c

CPU *cur_cpu(void)
{
    // The IF flag in the EFLAGS register permits all maskable hardware interrupts to be masked as a group
    if (readeflags() & FL_IF)
        PANIC("mycpu() called with interrupts enabled\n");
    return 0;

    int lapic_id = find_lapic_id();
    // APIC IDs are not guaranteed to be contiguous. Maybe we should have
    // a reverse map, or reserve a register to store &cpus[i].
    for (int i = 0; i < gNumCPUs; ++i)
    {
        if (gCPUs[i].apic_id == lapic_id)
            return &gCPUs[i];
    }
    PANIC("unknown APIC ID\n");
    return 0;
}

// In MP systems, the local APIC ID is also used as a processor ID
int cur_cpu_id()
{
    return cur_cpu() - gCPUs;
}