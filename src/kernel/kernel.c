#include <pic.h>
#include <screen.h>
#include <stdint.h>

#include "cpu/interrupt/idt.h"
#include "drivers/keyborad/keyborad.h"
#include "drivers/timer/timer.h"
#include "memory/pmm.h"

void kernel_main(void) {

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

  uint32_t frame = pmm_alloc_frame();
  screen_write(frame);

  // Enable interrupts
  __asm__ volatile("sti");

  while (1) {
    __asm__("hlt");
  }
}