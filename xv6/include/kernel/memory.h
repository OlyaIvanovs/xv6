#ifndef XV6_MEMORY_H
#define XV6_MEMORY_H

#include "base.h"

#define PTE_PS 0x0080 // Page Size

#define V2P(a) ((u32)(a)-KERNBASE)
#define P2V(a) ((void *)(((u8 *)(a)) + KERNBASE))

// ==================================== Types =====================================================

typedef u32 PDE; // Page directory entry
typedef u32 PTE; // Page table entry

// ==================================== Data ======================================================

extern u8 kernel_end; // first address after kernel loaded from ELF file(see linker script)

#define EXT_MEM 0x100000              // start of extended memory
#define PHYS_TOP 0x0E000000           // physical memory ends here (224 MB)
#define DEV_SPACE 0xFE000000          // devices at high addresses
#define KERNBASE 0x80000000           // first kernel virtual address
#define KERNLINK (KERNBASE + EXT_MEM) // address where the kernel is linked

#define V2P(a) ((u32)(a)-KERNBASE)
#define P2V(a) ((void *)(((u8 *)(a)) + KERNBASE))

#define PAGE_SIZE 4096 // 0x1000
#define NUM_PAGE_DIR_ENTRIES 1024

#define PTX_SHIFT 12 // offset of PTX in a linear address
#define PDX_SHIFT 22 // offset of PDX in a linear address

#define PAGE_DIR_INDEX(va) (((u32)(va) >> PDX_SHIFT) & 0x3FF)   // the first 10 bits of va 0x3FF ob1111111111
#define PAGE_TABLE_INDEX(va) (((u32)(va) >> PTX_SHIFT) & 0x3FF) // the second 10 bits of va

// Page table/directory entry flags
#define PTE_P 0x0001 // Present
#define PTE_W 0x0002 // Writeable
#define PTE_U 0x0004 // User

// Parsing the page dir/table entries
#define PTE_ADDR(pte) ((u32)(pte) & ~0xFFF)

// To ensure that it frees only aligned physical addresses
// A PTE can only refer to a physical address that is aligned on a 4096-byte boundary
#define ROUND_UP_PAGE(a) (u8 *)((u32)(a + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1))
#define ROUND_DOWN_PAGE(a) (u8 *)((u32)(a) & ~(PAGE_SIZE - 1))

// Segment selectors
#define SEG_KCODE 1 // kernel code
#define SEG_KDATA 2 // kernel data+stack
#define SEG_UCODE 3 // user code
#define SEG_UDATA 4 // user data+stack
#define SEG_TSS 5   // task state

// cpu->gdt[NSEGS] holds the above segments.
#define NSEGS 6

#ifndef __ASSEMBLER__
// Segment descriptor
typedef struct SegDescriptor
{
    // https://stackoverflow.com/questions/1604968/what-does-a-colon-in-a-struct-declaration-mean-such-as-1-7-16-or-32
    u32 lim_15_0 : 16;  // Low bits of segment limit
    u32 base_15_0 : 16; // Low bits of segment base address
    u32 base_23_16 : 8; // Middle bits of segment base address
    u32 type : 4;       // Segment type (0 - 5)
    u32 s : 1;          // Descriptor type (0 = system; 1 = code or data)
    u32 dpl : 2;        // Descriptor privilege level
    u32 p : 1;          // Segment present
    u32 lim_19_16 : 4;  // High bits of segment limit
    u32 avl : 1;        // Available for use by system software
    u32 rsv1 : 1;       // Reserved (64-bit code segment (IA-32e mode only))
    u32 db : 1;         // Default operation size 0 = 16-bit segment, 1 = 32-bit segment
    u32 g : 1;          // Granularity: limit scaled by 4K when set
    u32 base_31_24 : 8; // High bits of segment base address
} SegDescriptor;

#define SEG(type, base, lim, dpl)

#endif

// ==================================== Functions =================================================

void init_kernel_memory_range(void *vstart, void *vend);
void init_global_kernel_page_dir();
void switch_to_kernel_page_dir();
u8 *alloc_page();

#endif // XV6_MEMORY_H