#include "paging.h"
#include <memory.h>

#include "pmm.h"

// Load page directory
extern void load_page_directory(uint32_t *pd);

// Enable paging
extern void enable_paging();

// Kernel page directory
uint32_t *kernel_page_directory = 0;

// Initialize kernel paging
void paging_init()
{
    // Allocate page directory and page table

    uint32_t pd_location = pmm_alloc_frame();
    uint32_t pt_location = pmm_alloc_frame();

    uint32_t *pd = (uint32_t *)pd_location;
    uint32_t *pt = (uint32_t *)pt_location;

    // Clear memory
    memset(pd, 0, DIRECTORY_SIZE);
    memset(pt, 0, PAGE_SIZE);

    // Identity map first 4MB
    for (uint32_t i = 0; i < TOTAL_ENTRIES_IN_PAGE_TABLE; i++) {
        uint32_t addr = i * PAGE_SIZE;
        pt[i] = addr | PAGE_PRESENT | PAGE_RW;
    }

    // Link page table to directory
    pd[0] = pt_location | PAGE_PRESENT | PAGE_RW;

    // Store directory
    kernel_page_directory = pd;

    // Load page directory into CR3
    load_page_directory((uint32_t *)pd_location);

    // Enable paging
    enable_paging();
}