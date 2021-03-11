#include "base.h"
#include "kernel/traps.h"

#define IOAPIC_BASE 0xFEC00000 // Default physical address of IO APIC

#define REG_VER_INDEX 0x01  // Register index: version, also provides the maximum number of entries in the I/O Redirection Table.
#define REG_ID_INDEX 0x00   // Register index: This register contains the 4-bit APIC ID.
#define REG_TABLE_BASE 0x10 // Redirection table base. Each register is a dedicated entry for each interrupt input signal.

// The redirection table starts at REG_TABLE_BASE and uses 2 registers to configure each interrupt.
// The first (low) register in a pair contains configuration bits.
// The second (high) register contains a bitmask telling which CPUs can serve that interrupt.

#define INT_DISABLED 0x00010000  // Interrupt disabled. When 16th bit is 1, the interrupt signal is masked
#define INT_LEVEL 0x00008000     // Level-triggered (vs edge-)
#define INT_ACTIVELOW 0x00002000 // Active low (vs high)
#define INT_LOGICAL 0x00000800   // Destination is CPU id (vs APIC ID)

// IO APIC MMIO structure: write reg, then read or write data.
typedef struct IOAPIC
{
    u32 reg_index;
    u32 pad[3];
    u32 data;
} IOAPIC;

volatile IOAPIC *ioapic;
extern u8 gIoApicId;

static u32 ioapic_read(u32 reg_index)
{
    ioapic->reg_index = reg_index;
    return ioapic->data;
}

static void ioapic_write(u32 reg_index, u32 data)
{
    ioapic->reg_index = reg_index;
    ioapic->data = data;
}

void ioapic_init(void)
{
    ioapic = (volatile IOAPIC *)IOAPIC_BASE;
    // REG_VER provides the maximum number of entries in the I/O Redirection Table.
    // 23-16 bits contains the entry number (0 - 239)
    u32 max_entries = (ioapic_read(REG_VER_INDEX) >> 16) & 0xFF;
    // IOAPIC Identification—R/W. 27-24 bits contains the IOAPIC identification.
    u32 id = ioapic_read(REG_ID_INDEX) >> 24;

    if (id != gIoApicId)
    {
        // cprintf("ioapicinit: id isn't equal to ioapicid; not a MP\n"); // cprintf TODOD
    }

    for (int i = 0; i < max_entries; i++)
    {
        ioapic_write(REG_TABLE_BASE + 2 * i, INT_DISABLED | (T_IRQ0 + i)); // configuration bits. T_IRQ is the vector 8 bit field (from 10h to FEh)
        ioapic_write(REG_TABLE_BASE + 2 * i + 1, 0);
    }
}