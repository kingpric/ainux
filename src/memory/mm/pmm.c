#include <screen.h>
#include <stdint.h>

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
void pmm_reserve_range(uint32_t start_addr, uint32_t end_addr)
{
    for (uint32_t addr = start_addr; addr < end_addr; addr += FRAME_SIZE) {
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

// static void debug_print_e820()
// {
//     uint32_t count = *(uint32_t *)0x5000;
//     e820_entry_t *map = (e820_entry_t *)0x5004;

//     screen_write("E820 entries: ");
//     screen_write_dec(count);
//     screen_newline();

//     for (uint32_t i = 0; i < count; i++) {
//         screen_write("Base: ");
//         screen_write_hex((uint32_t)map[i].base);

//         screen_write("  Len: ");
//         screen_write_hex((uint32_t)map[i].length);

//         screen_write("  Type: ");
//         screen_write_dec(map[i].type);

//         screen_newline();
//     }
// }

// initialize bitmap with all frames marked as used
void pmm_init()
{
    for (uint32_t i = 0; i < MAX_FRAMES / 8; i++)
        bitmap[i] = 0xFF;

    pmm_mark_usable();

    // debug_print_e820();

    // screen_write("PMM: bitmap Location: ");
    // screen_write_hex((uintptr_t)bitmap);
    // screen_newline();

    // screen_write("PMM: total frames: ");
    // screen_write_dec(total_frames);
    // screen_newline();

    // screen_write("PMM: kernel start: ");
    // screen_write_hex((uintptr_t)&kernel_start);
    // screen_newline();

    // screen_write("PMM: kernel end: ");
    // screen_write_hex((uintptr_t)&kernel_end);
    // screen_newline();

    // pmm_reserve_range((uintptr_t)&kernel_start, (uintptr_t)&kernel_end);
    // pmm_reserve_range(0x00000000, 0x00100000);

    pmm_reserve_range(0x00000000, 0x00100000);                             // BIOS
    pmm_reserve_range((uintptr_t)&kernel_start, (uintptr_t)&kernel_end);   // kernel
    pmm_reserve_range(PMM_BITMAP_ADDR, PMM_BITMAP_ADDR + PMM_BITMAP_SIZE); // bitmap
}
