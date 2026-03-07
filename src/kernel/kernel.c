#include <stdint.h>
#include <screen.h>
#include <pic.h>

#include "cpu/interrupt/idt.h"
#include "drivers/timer/timer.h"

void kernel_main(void)
{
    /* Initialize screen or vga*/
    screen_init();
    screen_write("Hello I'm Ainux\n");

    /* Initialize Interrupt system*/
    idt_init();

    /* Initialize system timer*/
    timer_init();

    // Enable interrupts
    __asm__ volatile("sti");

    while (1) {
        __asm__("hlt");
    }
}