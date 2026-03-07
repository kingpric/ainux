#include <stdint.h>
#include <screen.h>
#include <io.h>
#include <pic.h>
#include <config.h>

#include "idt.h"
#include "memory/memory.h"
#include "interrupt.h"

extern void idt_load(idtr_t* idtr);

/* ISR declarations from assembly */
extern void *interrupt_pointer_table[INTERRUPT_SIZE];

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
    init_interrupts();

    /* Clear IDT */
    memset(idt, 0, sizeof(idt));
    memset(&idtr, 0, sizeof(idtr));
    
    /* Set IDT limit and base */
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint32_t)&idt;

    for (int i = 0; i < INTERRUPT_SIZE; i++)
    {
        idt_set_gate(i, interrupt_pointer_table[i]);
    }

    /* */
    idt_load(&idtr);

    /* */
    pic_remap();



}

