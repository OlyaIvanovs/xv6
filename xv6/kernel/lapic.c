// The local APIC manages internal interrupts (including interprocessor interrupts)
// See Chapter 10 of Intel manual vol. 3

#include "base.h"
#include "kernel/traps.h"

// Local APIC registers, divided by 4 for use as u32[] indices.
#define ID (0x0020 / 4)            // Local APIC ID Register
#define VERSION (0x0030 / 4)       // Local APIC Version Register
#define TASK_PRIORITY (0x0080 / 4) // Task Priority Register (TPR)
#define EOI (0x00B0 / 4)
#define SPURIOUS_INTERRUPT_VECTOR (0x00F0 / 4)
#define UNIT_ENABLE 0x00000100
#define ERROR_STATUS (0x0280 / 4)
#define INIT_RESET 0x00000500
#define STARTUP_IPI 0x00000600
#define DELIVERY_STATUS 0x00001000
#define ASSERT 0x00004000 // Assert interrupt (vs deassert)
#define DEASSERT 0x00000000
#define LEVEL_TRIGGERED 0x00008000
#define BROADCAST 0x00080000 // Send to all APICs, including self.
#define BUSY 0x00001000
#define FIXED 0x00000000
#define INTERRUPT_COMMAND_LO (0x0300 / 4) // Interrupt Command [31:0]
#define INTERRUPT_COMMAND_HI (0x0310 / 4) // Interrupt Command [63:32]
#define TIMER (0x0320 / 4)                // Local Vector Table 0 (TIMER)
#define PCINT (0x0340 / 4)                // Performance Counter LVT
#define LINT0 (0x0350 / 4)                // Local Vector Table 1 (LINT0)
#define LINT1 (0x0360 / 4)                // Local Vector Table 2 (LINT1)
#define ERROR (0x0370 / 4)                // Local Vector Table 3 (ERROR)

#define MASKED 0x00010000   // Interrupt masked
#define PERIODIC 0x00020000 // Periodic

#define DIVIDE_COUNTS_BY_1 0x0000000B
#define TIMER_INITIAL_COUNT (0x0380 / 4)
#define TIMER_CURRENT_COUNT (0x0390 / 4) // Read only
#define TIMER_DIVIDE_CONFIG (0x03E0 / 4)

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

    lapic_write(SPURIOUS_INTERRUPT_VECTOR, UNIT_ENABLE | (T_IRQ0 + IRQ_SPURIOUS));

    // Configure timer interrupts (every 10000000 ticks at bus frequency)
    lapic_write(TIMER_DIVIDE_CONFIG, DIVIDE_COUNTS_BY_1);
    lapic_write(TIMER, PERIODIC | (T_IRQ0 + IRQ_SPURIOUS));
    lapic_write(TIMER_INITIAL_COUNT, 10000000);
}