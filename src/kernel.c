#include <stdint.h>

void kernel_main(void)
{
    volatile uint16_t* vga = (uint16_t*)0xB8000;

    vga[0] = 0x0F41;  // 'A' with white on black

    while (1) {
        // halt CPU to reduce heat
        __asm__("hlt");
    }
}