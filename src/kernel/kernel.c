#include <pic.h>
#include <screen.h>
#include <stdint.h>

#include "cpu/interrupt/idt.h"
#include "drivers/keyborad/keyborad.h"
#include "drivers/timer/timer.h"
#include "memory/mm/paging.h"
#include "memory/mm/pmm.h"

void kernel_main(void)
{

    /* Initialize screen or vga*/
    screen_init();

    screen_write("Hello I'm Ainux\n");

    /* Initialize Interrupt system*/
    idt_init();

    /* Initialize PIC*/
    pic_remap();

    /* Initialize system timer*/
    timer_init();

    /* Initialize keyboard*/
    keyboard_init();

    /* Initialize physical memory manager*/
    pmm_init();
    screen_write("PMM initialized\n");

    /* Initialize paging*/
    paging_init();
    screen_write("Paging initialized\n");

    // testing frame alloc
    uint32_t frame = pmm_alloc_frame();
    screen_write("Frame: ");
    screen_write_hex(frame);
    screen_put_char('\n');

    // Enable interrupts
    __asm__ volatile("sti");

    while (1) {
        __asm__("hlt");
    }
}