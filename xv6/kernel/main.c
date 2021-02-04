#include "kernel/memory.h"

void init_global_kernel_page_dir();
void switch_to_kernel_page_dir();

void kernel_start()
{
    // Changes to a new page table for the kernel to run
    // init_global_kernel_page_dir();
    // switch_to_kernel_page_dir();
}

// Page directory must be page-aligned.
// Since we're specifying the PTE_PS flag for the directory entries,
// it treats them as references to 4MB pages
__attribute__((__aligned__(PAGE_SIZE))) u32 entry_page_dir[NUM_PAGE_DIR_ENTRIES] = {
    // Map VA [0 : 4MB] to PA [0 : 4MB]
    [0] = 0 | PTE_P | PTE_W | PTE_PS,

    // Map VA [KERNBASE : KERNBASE + 4MB] to PA [0 : 4MB]
    [KERNBASE >>
        PDX_SHIFT] = 0 | PTE_P | PTE_W | PTE_PS,
};