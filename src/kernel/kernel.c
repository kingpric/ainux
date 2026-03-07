#include <stdint.h>
#include "drivers/screen/screen.h"

void kernel_main(void)
{
    
    screen_init();
    // for (int i = 0; i < 30; i++)
    // {
    //     screen_write("Scrolling line\n");
    // }
    screen_write("Hello World!");

    while (1) {
        __asm__("hlt");
    }
}