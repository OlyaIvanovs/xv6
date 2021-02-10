#include "kernel/memory.h"

// ==================================== Internal types ============================================

typedef struct FreePage
{
    struct FreePage *next;
} FreePage;

typedef struct KMemory
{
    Spinlock lock;
    FreePage *free_list;
    bool use_lock;
} KMemory;

// ==================================== Globals ===================================================

extern u8 kernel_data; // defined by linker

static KMemory gKMemory;

//
void free_page(void *va)
{
    // Validate the address
    if ((u32)va % PAGE_SIZE != 0)
    {
        PANIC("free_page: va is not page-aligned");
    }
    if ((u8 *)va < &kernel_end || V2P(va) >= PHYS_TOP)
    {
        PANIC("free_page: va is out of range");
    }

    // Fill with 1 to catch references to freed memory
    memset(va, 1, PAGE_SIZE);

    acquire(&gKMemory.lock);

    // Add the va on free list
    FreePage *list = (FreePage *)va;
    list->next = gKMemory.free_list;
    gKMemory.free_list = list;

    release(&gKMemory.lock);
}

void init_kernel_memory_range(void *vstart, void *vend)
{
    init_lock(&gKMemory.lock, "gKMemory");

    // Add a range of virtual addresses on free list.
    for (u8 *page = ROUND_UP_PAGE(vstart); page + PAGE_SIZE <= (u8 *)vend; page += PAGE_SIZE)
    {
        free_page(page);
    }
}