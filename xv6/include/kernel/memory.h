#ifndef XV6_MEMORY_H
#define XV6_MEMORY_H

#include "base.h"

#define PTE_PS 0x0080 // Page Size

#define V2P(a) ((u32)(a)-KERNBASE)
#define P2V(a) ((void *)(((u8 *)(a)) + KERNBASE))

// ==================================== Data ======================================================

extern u8 kernel_end; // first address after kernel loaded from ELF file(see linker script)

#define KERNBASE 0x80000000 // first kernel virtual address
#define PHYS_TOP 0x0E000000 // physical memory ends here (224 MB)

#define PAGE_SIZE 4096 // 0x1000
#define NUM_PAGE_DIR_ENTRIES 1024

#define PTX_SHIFT 12 // offset of PTX in a linear address
#define PDX_SHIFT 22 // offset of PDX in a linear address

// Page table/directory entry flags
#define PTE_P 0x0001 // Present
#define PTE_W 0x0002 // Writeable
#define PTE_U 0x0004 // User

// To ensure that it frees only aligned physical addresses
// A PTE can only refer to a physical address that is aligned on a 4096-byte boundary
#define ROUND_UP_PAGE(a) (u8 *)((u32)(a + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1))

// ==================================== Functions =================================================

void init_kernel_memory_range(void *vstart, void *vend);

#endif // XV6_MEMORY_H