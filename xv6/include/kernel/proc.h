#ifndef XV6_PROC_H
#define XV6_PROC_H

#include "base.h"

// Per-CPU state
typedef struct CPU
{
    u8 apic_id; // Local APIC ID

    // TODO
} CPU;

typedef struct KernelContext
{
    u8 edi;
    u8 esi;
    u8 ebx;
    u8 ebp;
    u8 eip;
} KernelContext;

// Per-process state
typedef struct Process
{

} Process;

#endif // XV6_PROC_H