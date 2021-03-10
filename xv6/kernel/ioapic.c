#include "base.h"

#define IOAPIC_BASE 0xFEC00000 // Default physical address of IO APIC

#define REG_VER_INDEX 0x01 // Register index: version, also provides the maximum number of entries in the I/O Redirection Table.

volatile IOAPIC *ioapic;

// IO APIC MMIO structure: write reg, then read or write data.
typedef struct IOAPIC
{
    u32 reg_index;
    u32 pad[3];
    u32 data;
} IOAPIC;

static u32 ioapic_read(u32 reg_index)
{
    ioapic->reg = reg;
    return ioapic->data;
}

void ioapic_init(void)
{
    ioapic = (volatile IOAPIC *)IOAPIC_BASE;
    // REG_VER provides the maximum number of entries in the I/O Redirection Table.
    // 23-16 bits contains the entry number (0 - 239)
    u32 max_entries = (ioapic_read(REG_VER_INDEX) >> 16) & 0xFF;
}