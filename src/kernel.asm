[BITS 32]

global _start
extern kernel_main

section .text

_start:
    ; mov esp, 0x200000      ; Set stack (2MB)

    call kernel_main

.hang:
    hlt
    jmp .hang