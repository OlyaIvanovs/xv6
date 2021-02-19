// https://pdos.csail.mit.edu/6.828/2008/readings/ia32/MPspec.pdf

#include "kernel/mp.h"
#include "base.h"

// Search for an MP configuration table.
static bool find_mp_config(MP_FPStruct **p_mp_struct, MP_ConfigTable **p_mp_table)
{
}

void mp_init()
{
    MP_FPStruct *mp_fp_struct;
    MP_ConfigTable *config;

    bool found = find_mp_config(&mp_fp_struct, &config);
}