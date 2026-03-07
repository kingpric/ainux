[BITS 32]

global isr32
extern interrupt_handler

isr32:
    cli
    pusha          ; save registers
    push 32        ; interrupt number

    call interrupt_handler

    add esp, 4     ; remove interrupt number
    popa
    sti
    iret