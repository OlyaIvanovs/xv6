// https://pdos.csail.mit.edu/6.828/2008/readings/ia32/MPspec.pdf

#include "kernel/mp.h"
#include "base.h"

// Search for the floating pointer structure in the locations defined in spec
static MP_FPStruct *mp_fp_struct_search()
{
    u8 *BDA = (u8 *)P2V(0x400); // BIOS Data Area

    BDA[0x0F] == *(BDA + 0x0F);

    // Check the first KB of the extended BIOS data area (EBDA)
    u32 addr = ((BDA[0x0F] << 8) | BDA[0x0E]) << 4;
}

// Search for an MP configuration table.
static bool find_mp_config(MP_FPStruct **p_mp_struct, MP_ConfigTable **p_mp_table)
{
    // Find the FP struct
    MP_FPStruct *mp_struct = mp_fp_struct_search();
}

// Search for an MP configuration table.
static bool find_mp_config(MP_FPStruct **p_mp_struct, MP_ConfigTable **p_mp_table)
{
    // Find the FP Struct
    MP_FPStruct *mp_struct = mp_fp_struct_search();
}

void mp_init()
{
    MP_FPStruct *mp_fp_struct;
    MP_ConfigTable *config;

    // Try to find an MP config table
    bool found = find_mp_config(&mp_fp_struct, &config);
}