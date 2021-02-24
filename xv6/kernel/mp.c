// https://pdos.csail.mit.edu/6.828/2008/readings/ia32/MPspec.pdf

#include "base.h"
#include "string.h"
#include "x86_asm.h"
#include "kernel/mp.h"
#include "kernel/console.h"
#include "kernel/memory.h"
#include "kernel/proc.h"
#include "kernel/param.h"

//==================================== Globals ==================================//
CPU gCPUs[MAX_NUM_CPUS];
int gNumCPUs = 0;
u8 gIoApicId;

extern u32 *gLAPIC; // defined in kernel/lapic.c

//==================================== Functions ================================//

// Search for the floating pointer struct at [addr : addr + len bytes]
static MP_FPStruct *mp_search(u32 phys_addr, int len)
{
    u8 *addr = P2V(phys_addr);
    for (u8 *p = addr; p < addr + len; p += sizeof(MP_FPStruct))
    {
        // check signature
        if (memcmp(p, "_MP_", 4) == 0)
        {
            // checksum: all bytes must add up to 0
            if (sum_bytes(p, sizeof(MP_FPStruct)) == 0)
            {
                LOG_INFO("Found at address %p", p);
                return (MP_FPStruct *)p;
            }
            else
            {
                LOG_ERROR("MP struct with a wrong checksum at %p", p);
            }
        }
    }
    LOG_INFO("mp_search: Not found");
    return NULL;
}

// Search for the floating pointer structure in the locations defined in spec
static MP_FPStruct *mp_fp_struct_search()
{
    u8 *BDA = (u8 *)P2V(0x400); // BIOS Data Area

    {
        // Check the first KB of the extended BIOS data area (EBDA)
        // 0x040E BDA (word) is EBDA base address >> 4 (usually!)
        u32 addr = ((BDA[0x0F] << 8) | BDA[0x0E]) << 4;
        MP_FPStruct *result = mp_search(addr, 1024);
        if (result)
        {
            return result;
        }
    }

    {
        // Check the last KB of system base memory
        u32 addr = ((BDA[0x14] << 8) | BDA[0x13]) * 1024;
        MP_FPStruct *result = mp_search(addr - 1024, 1024);
        if (result)
        {
            return result;
        }
    }

    // Check the BIOS ROM between 0x0F0000 and 0x0FFFFF
    return mp_search(0xF0000, 0x10000);
}

// Search for an MP configuration table.
static bool find_mp_config(MP_FPStruct **p_mp_struct, MP_ConfigTable **p_mp_table)
{
    // Find the FP Struct
    MP_FPStruct *mp_struct = mp_fp_struct_search();
    if (mp_struct == 0)
    {
        LOG_ERROR("Unable to find the floating pointer struct");
        return false;
    }
    if (mp_struct->config_table_phys_addr == 0)
    {
        LOG_ERROR("We do not support default MP configurations");
        return false;
    }

    // Get the configuration table
    MP_ConfigTable *config = (MP_ConfigTable *)P2V(mp_struct->config_table_phys_addr);
    // Check for correct signature, calculate the checksum and, if correct, check the version.
    if (memcmp(config->signature, "PCMP", 4) != 0)
    {
        LOG_ERROR("Invalid MP config table signature");
        return false;
    }
    if (config->version != 1 && config->version != 4)
    {
        LOG_ERROR("Unsupported config version: %d", config->version);
        return false;
    }
    if (sum_bytes((u8 *)config, config->length) != 0)
    {
        LOG_ERROR("Invalid MP config table checksum");
        return false;
    }

    // Found everything
    *p_mp_struct = mp_struct;
    *p_mp_table = config;
    return true;
}

void mp_init()
{
    MP_FPStruct *mp_fp_struct;
    MP_ConfigTable *config;

    // Try to find an MP config table
    bool found = find_mp_config(&mp_fp_struct, &config);
    if (!found)
        PANIC("Couldn't find a valid MP config table. Probably not an SMP system.");

    // The base address by which each processor accesses its local APIC.
    gLAPIC = (u32 *)config->lapic_addr;

    // Go through the config table entries
    for (u8 *entry = (u8 *)(config + 1); entry < (u8 *)config + config->length;)
    {
        switch (*entry)
        {
        case MP_ENTRY_PROC:
        {
            MP_ProcEntry *proc = (MP_ProcEntry *)entry;
            if (gNumCPUs < MAX_NUM_CPUS)
            {
                gCPUs[gNumCPUs].apic_id = proc->apic_id;
                gNumCPUs++;
            }
            entry += sizeof(*proc);
        }
        break;
        case MP_ENTRY_IOAPIC:
        {
            MP_IoApicEntry *io_apic = (MP_IoApicEntry *)entry;
            gIoApicId = io_apic->apic_id;
            entry += sizeof(*io_apic);
        }
        break;
        case MP_ENTRY_BUS:
        case MP_ENTRY_IOINTR:
        case MP_ENTRY_LINTR:
        {
            entry += 8;
        }
        break;
        // Ignore other entries
        default:
        {
            PANIC("Incorrect entry in the MP config table");
        }
        break;
        }
    }

    // when IMCR is present, PIC Mode is implemented;
    // IMCR - interrupt mode configuration register
    // To access the IMCR, write a value of 70h to I/O port 22h, which
    // selects the IMCR. Then write the data to I/O port 23h.
    // The power-on default value is zero, which connects the NMI and
    // 8259 INTR lines directly to the BSP. Writing a value of 01h forces the
    // NMI and 8259 INTR signals to pass through the APIC.
    if (mp_fp_struct->imcr_present)
    {
        // Bochs doesn't support IMCR, so this doesn't run on Bochs
        out_u8(0x22, 0x70);            // select IMCR
        out_u8(0x23, in_u8(0x23) | 1); // mask external interrupts
    }
}