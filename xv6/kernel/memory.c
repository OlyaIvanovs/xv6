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

// Frees the page of physical memory where va points to
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

    // Fill with 00010001 to catch references to freed memory
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

// ORIGINAL

void kinit1(void *vstart, void *vend)
{
    initlock(&kmem.lock, "kmem");
    kmem.use_lock = 0;
    freerange(vstart, vend);
}

void freerange(void *vstart, void *vend)
{
    char *p;
    p = (char *)PGROUNDUP((uint)vstart);
    for (; p + PGSIZE <= (char *)vend; p += PGSIZE)
        kfree(p);
}

struct
{
    struct spinlock lock;
    int use_lock;
    struct run *freelist;
} kmem;

//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(char *v)
{
    struct run *r;

    if ((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
        panic("kfree");

    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);

    if (kmem.use_lock)
        acquire(&kmem.lock);
    r = (struct run *)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
    if (kmem.use_lock)
        release(&kmem.lock);
}