#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#include "cpu/interrupt/interrupt.h"
#include "pmm.h"

#define PAGE_SIZE FRAME_SIZE      // 4KB
#define DIRECTORY_SIZE FRAME_SIZE // 4KB

#define TOTAL_ENTRIES_IN_PAGE_TABLE 1024
#define TOTAL_ENTRIES_IN_PAGE_DIRECTORY 1024

#define PAGE_DIR_VIRT 0xFFFFF000
#define PAGE_TABLES_VIRT 0xFFC00000

/*
 * Helpers to access paging structures
 */
#define GET_PAGE_DIRECTORY() ((uint32_t *)PAGE_DIR_VIRT)

#define GET_PAGE_TABLE(i) ((uint32_t *)(PAGE_TABLES_VIRT + ((i) * 0x1000)))

/*
 * Paging structures
 */

typedef uint32_t page_directory_t[PAGE_SIZE];
typedef uint32_t page_table_t[PAGE_SIZE];

// Page flags
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLE 0x10

// Global page directory pointer
// extern uint32_t *kernel_page_directory;

// Initialize paging
void paging_init();

// Page fault handler
void page_fault_handler(interrupt_frame_t *frame);

void page_test();

#endif