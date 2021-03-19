#include "kernel/memory.h"
#include "kernel/mp.h"
#include "kernel/lapic.h"
#include "kernel/ioapic.h"
#include "kernel/uart.h"
#include "kernel/console.h"

void init_global_kernel_page_dir();
void switch_to_kernel_page_dir();
void pic_init();

void kernel_start()
{
    // The allocator refers to physical pages by their virtual addresses as mapped in high
    // memory, not by their physical addresses, which is why init_kernel_memory_range() uses P2V(PHYSTOP) to
    // translate PHYSTOP (a physical address) to a virtual address.
    init_kernel_memory_range(&kernel_end, P2V(4 * 1024 * 1024));

    init_global_kernel_page_dir();
    switch_to_kernel_page_dir();

    mp_init(); // detect other processors

    // Init the local interrupt controller
    lapic_init();

    // Init segment descriptors
    segments_init();

    // Disable PIC
    pic_init();

    ioapic_init();

    console_init();

    uart_init();
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