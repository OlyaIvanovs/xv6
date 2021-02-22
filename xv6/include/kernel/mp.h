#ifndef XV6_MP_H
#define XV6_MP_H

#include "base.h"

// Floating pointer struct
typedef struct MP_FPStruct
{
    u8 signature[4];              // must be equal to "_MP_"
    void *config_table_phys_addr; // contains the address of MP_ConfigTable if present
    u8 length;                    // the length of the floating pointer structure table
    u8 spec_rev;                  // The version number of the MP specification supported
    u8 checksum;                  // all bytes must add up to 0
    u8 sys_config_type;           // MP System Configuration Type
    u8 imcr_present;              // when 1 the IMCR is present and PIC Mode is implemented;
                                  // otherwise, Virtual Wire Mode is implemented
                                  //  IMCR - interrupt mode configuration register
    u8 reserved[3];
} MP_FPStruct;

// Config table header
typedef struct MP_ConfigTable
{
    u8 signature[4]; // must be equal to PCMP
    u16 length;      // The length of the base configuration table in bytes.
                     // It aids in calculation of the checksum
    u8 version;      // version number of the MP specification
    u8 checksum;
    u8 oem_id[8];       // the manufacturer of the system hardware
    u8 product_id[12];  //  identifies the product family.
    u32 *oem_table;     // A physical-address pointer to an OEM-defined configuration table
    u16 oem_table_size; // The size of the base OEM table in bytes
    u16 entry_count;    // The number of entries in the variable portion of the base table
    u32 *lapic_addr;    // The base address by which each processor accesses its local APIC.
    u16 ext_table_length;
    u8 ext_table_checksum;
    u8 reserved;
} MP_ConfigTable;

//  format of processor entry
typedef struct MP_ProcEntry
{
    u8 type;           // entry type (0 for proc)
    u8 apic_id;        // local APIC id
    u8 apic_version;   // local APIC version
    u8 flags;          // CPU flags
    u8 signature[4];   // CPU signature
    u32 feature_flags; // feature flags from CPUID instaruction
    u8 reserved[8];
} MP_ProcEntry;

typedef struct MP_IoApicEntry
{
    u8 type;    // entry type (2 for io apic)
    u8 apic_id; // IO APIC ID
    u8 version; // IO APIC Version
    u8 flags;   // If 0, this I/O APIC is unusable, OS should not attempt to access this I/O APIC.
    u32 *addr;  // Base address for this I/O APIC
} MP_IoApicEntry;

// Table entry types
#define MP_ENTRY_PROC 0x00   // One entry per processor.
#define MP_ENTRY_BUS 0x01    // One entry per bus.
#define MP_ENTRY_IOAPIC 0x02 // One entry per I/O APIC.
#define MP_ENTRY_IOINTR 0x03 // One entry per bus interrupt source.
#define MP_ENTRY_LINTR 0x04  // One entry per system interrupt source.

void mp_init();

#endif // XV6_MEMORY_H