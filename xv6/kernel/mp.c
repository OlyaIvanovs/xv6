// https://pdos.csail.mit.edu/6.828/2008/readings/ia32/MPspec.pdf

#include "kernel/mp.h"
#include "base.h"

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
        LOG_INFO("mp_search: Not found");
        return NULL;
    }
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

    // Check the BIOS ROM between 0xE0000 and 0xFFFFF
    // TODO !!(mpsearch1(0xF0000, 0x10000))
    return mp_search(0xE0000, 0x20000);
}

// Search for an MP configuration table.
static bool find_mp_config(MP_FPStruct **p_mp_struct, MP_ConfigTable **p_mp_table)
{
    // Find the FP struct
    MP_FPStruct *mp_struct = mp_fp_struct_search();
    if (mp_struct == 0)
    {
        LOG_ERROR("Unable to find the floating pointer struct");
        return false;
    }
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
}

void mp_init()
{
    MP_FPStruct *mp_fp_struct;
    MP_ConfigTable *config;

    // Try to find an MP config table
    bool found = find_mp_config(&mp_fp_struct, &config);
}