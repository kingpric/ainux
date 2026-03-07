#include <stdint.h>
#include <screen.h>
#include "cpu/idt/idt.h"
#include <pic.h>

void kernel_main(void)
{
    
    screen_init();
    // for (int i = 0; i < 30; i++)
    // {
    //     screen_write("Scrolling line\n");
    // }
    screen_write("Hello again!\n");

    // Initialize IDT
    idt_init();

    screen_write("idt initialized\n");

    pic_remap();

    screen_write("pic remapped\n");

    __asm__ volatile("sti");

    screen_write("sti enabled\n");

    while (1) {
        __asm__("hlt");
    }
}