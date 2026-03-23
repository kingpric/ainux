
#include <memory.h>
#include <screen.h>
#include <stdint.h>

#include "paging.h"
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
    // Allocate page directory
    uint32_t pd_location = pmm_alloc_frame();
    uint32_t *pd = (uint32_t *)pd_location;

    // Allocate page table
    uint32_t pt_location = pmm_alloc_frame();
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

    // Recursive mapping of page directory
    pd[1023] = pd_location | PAGE_PRESENT | PAGE_RW;

    // Store directory
    kernel_page_directory = pd;

    // Load page directory into CR3
    load_page_directory((uint32_t *)pd_location);

    // Enable paging
    enable_paging();

    // Register page fault handler
    register_interrupt_handler(14, page_fault_handler);
}

void page_fault_handler(interrupt_frame_t *frame)
{

    uint32_t error_code = frame->error_code;

    uint32_t faulting_address;

    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

    screen_write("\nPAGE FAULT!\n");

    screen_write("Faulting Address: ");
    screen_write_hex(faulting_address);
    screen_newline();

    screen_write("Error Code: ");
    screen_write_hex(error_code);
    screen_newline();

    if (!(error_code & 0x1))
        screen_write("Reason: Page not present\n");

    if (error_code & 0x2)
        screen_write("Reason: Write operation\n");

    if (error_code & 0x4)
        screen_write("Reason: User mode access\n");

    screen_write("System halted.\n");

    while (1) {
    }
}

void page_test()
{

    // screen_write("Mapping a new page for virtual address: ");
    // screen_write_hex(0x543000);
    // screen_newline();

    kernel_page_directory[1023] = (uint32_t)kernel_page_directory | PAGE_PRESENT | PAGE_RW;

    uint32_t pt_new_phy;

    // keep allocating until > 4MB
    do {
        pt_new_phy = pmm_alloc_frame();
    } while (pt_new_phy < 0x00400000);

    screen_write("New Frame Physical Address: ");
    screen_write_hex(pt_new_phy);
    screen_newline();

    kernel_page_directory[1] = pt_new_phy | PAGE_PRESENT | PAGE_RW;

    screen_write("Page directory entry 1: ");
    screen_write_hex(kernel_page_directory[1]);
    screen_newline();

    // Try to initialize page table
    // uint32_t *pt = (uint32_t *)pt_new_phy;
    uint32_t *pt = (uint32_t *)(0xFFC00000 + (1 * 0x1000));
    memset(pt, 0, PAGE_SIZE);
    pt[0] = 0x12345000 | PAGE_PRESENT | PAGE_RW;

    screen_write("Page table entry 0: ");
    screen_write_hex(pt[0]);
    screen_newline();
}
