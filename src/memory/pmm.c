#include "pmm.h"

#define PMM_BITMAP_ADDR 0x200000
#define E820_MAP_ADDR 0x5000

extern uint32_t kernel_start;
extern uint32_t kernel_end;

static uint8_t *bitmap = (uint8_t *)PMM_BITMAP_ADDR;
static uint32_t total_frames = MAX_FRAMES;

static inline void set_frame(uint32_t frame)
{
    bitmap[frame / 8] |= (1 << (frame % 8));
}

static inline void clear_frame(uint32_t frame)
{
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static inline int test_frame(uint32_t frame)
{
    return bitmap[frame / 8] & (1 << (frame % 8));
}

// mark all frames as free that are marked as usable in e820 map
void pmm_mark_usable()
{
    uint32_t count = *(uint32_t *)E820_MAP_ADDR;
    e820_entry_t *map = (e820_entry_t *)(E820_MAP_ADDR + 4);

    for (uint32_t i = 0; i < count; i++) {
        if (map[i].type != 1)
            continue;

        uint64_t start = map[i].base;
        uint64_t end = start + map[i].length;

        for (uint64_t addr = start; addr < end; addr += FRAME_SIZE) {
            uint32_t frame = addr / FRAME_SIZE;
            clear_frame(frame);
        }
    }
}

// mark all frames as used that are reserved for kernel
void pmm_reserve_kernel(uint32_t kernel_start, uint32_t kernel_end)
{
    for (uint32_t addr = kernel_start; addr < kernel_end; addr += FRAME_SIZE) {
        uint32_t frame = addr / FRAME_SIZE;
        set_frame(frame);
    }
}

// allocate a free frame
uint32_t pmm_alloc_frame()
{
    for (uint32_t frame = 0; frame < total_frames; frame++) {
        if (!test_frame(frame)) {
            set_frame(frame);
            return frame * FRAME_SIZE;
        }
    }

    return 0;
}

void pmm_free_frame(uint32_t addr)
{
    uint32_t frame = addr / FRAME_SIZE;
    clear_frame(frame);
}

// initialize bitmap with all frames marked as used
void pmm_init()
{
    for (uint32_t i = 0; i < MAX_FRAMES / 8; i++)
        bitmap[i] = 0xFF;

    pmm_mark_usable();

    pmm_reserve_kernel((uintptr_t)&kernel_start, (uintptr_t)&kernel_end);
}