// The local APIC manages internal interrupts (including interprocessor interrupts)
// See Chapter 10 of Intel manual vol. 3

#include "base.h"
#include "kernel/traps.h"

// Local APIC registers, divided by 4 for use as u32[] indices.
#define ID (0x0020 / 4) // Local APIC ID Register

volatile u32 *gLAPIC; // initialised in kernel/mp.c

static void lapic_write(int index, int value)
{
    // Talk to the memory-mapped local APIC
    gLAPIC[index] = value;
    gLAPIC[ID]; // wait for the write to finish, by reading
}

void lapic_init()
{
    if (!gLAPIC)
    {
        return;
    }

    lapic_write();
}