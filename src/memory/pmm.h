#ifndef PMM_H
#define PMM_H

#include <stddef.h>
#include <stdint.h>

// Size of each physical memory frame in bytes (4 KB).
#define FRAME_SIZE 4096

// Maximum amount of memory supported (4 GB). ULL -> Unsigned Long Long
#define MAX_MEMORY (4 * 1024 * 1024 * 1024ULL)

// Maximum number of frames based on maximum supported memory.
#define MAX_FRAMES (MAX_MEMORY / FRAME_SIZE)

// Size of the physical memory manager bitmap in bytes.
// A single bit in the bitmap represents one frame.
// (e.g., 0 means free, 1 means occupied).
#define PMM_BITMAP_SIZE (MAX_FRAMES / 8)

// e820 memory map entry structure.
typedef struct {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t attr;
} e820_entry_t;

// Initializes the physical memory manager.
void pmm_init();

// Allocates a single physical memory frame.
// Returns Physical address of the allocated frame.
uint32_t pmm_alloc_frame();

// Frees a previously allocated physical memory frame.
// addr: Physical address of the frame to free.
void pmm_free_frame(uint32_t addr);

#endif