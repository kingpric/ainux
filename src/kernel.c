#include <stdint.h>
#include "drivers/screen/screen.h"

void kernel_main(void)
{
    
    screen_init();
    screen_write("Hello, World!\n");
    screen_write("Line 1\n");
    screen_write("Line 2\n");
    screen_write("Line 3\n");

    while (1) {
        // halt CPU to reduce heat
        __asm__("hlt");
    }
}