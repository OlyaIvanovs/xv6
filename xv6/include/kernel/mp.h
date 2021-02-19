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
    u16 length;
    u8 version; // version number of the MP specification
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

void mp_init();

#endif // XV6_MEMORY_H