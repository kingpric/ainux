#include <stdint.h>
#include <screen.h>
#include <io.h>
#include "idt.h"
#include "config.h"

extern void idt_load(idtr_t* idtr);

/* ISR declarations from assembly */
extern void isr32();

static idt_entry_t idt[INTERRUPT_SIZE];
static idtr_t idtr;

/*
 * Set IDT gate
 */
void idt_set_gate(uint8_t interrupt_no, void (*handler)())
{
    idt[interrupt_no].offset_low  = (uint32_t)handler & 0xFFFF;
    idt[interrupt_no].selector    = KERNEL_CODE_SELECTOR;       // kernel code segment
    idt[interrupt_no].zero        = 0;
    idt[interrupt_no].type_attr   = 0x8E;                       // interrupt gate
    idt[interrupt_no].offset_high = ((uint32_t)handler >> 16) & 0xFFFF;
}

/*
 * Initialize IDT
 */
void idt_init()
{
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint32_t)&idt;

    /* Install timer interrupt */
    idt_set_gate(32, isr32);

    idt_load(&idtr);
}

void interrupt_handler(uint32_t interrupt)
{
    if (interrupt == 32)
    {
        screen_write(".");
    }

    /* Send EOI */
    outb(0x20, 0x20);
}