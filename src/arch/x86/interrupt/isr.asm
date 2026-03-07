[BITS 32]

; global isr32
; extern interrupt_handler

; isr32:
;     pusha                  ; Save all general purpose registers      

;     push 32                ; Push the interrupt number      

;     mov eax, esp           ; Move the stack pointer to EAX      
;     push eax               ; Push the stack pointer to the stack

;     call interrupt_handler ; Call the interrupt handler

;     add esp, 4             ; Remove the interrupt number
;     add esp, 4             ; Remove the stack pointer

;     popa                   ; Restore all general purpose registers
;     iret                   ; Return from interrupt


extern interrupt_handler

global isr_common_stub

; ==============================
; Common ISR entry
; ==============================

isr_common_stub:

    pusha                       

    mov eax, esp
    push eax

    call interrupt_handler

    add esp, 4

    popa

    add esp, 8

    iret


; ==============================
; Macros
; ==============================

%macro ISR_NOERR 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro


%macro ISR_ERR 1
global isr%1
isr%1:
    push %1
    jmp isr_common_stub
%endmacro


; ==============================
; CPU Exceptions
; ==============================
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31


; ==============================
; Hardware IRQs (PIC remapped)
; ==============================
%assign i 32
%rep 15
    ISR_NOERR i
%assign i i+1
%endrep
; ISR_NOERR 32
; ISR_NOERR 33
; ISR_NOERR 34
; ISR_NOERR 35
; ISR_NOERR 36
; ISR_NOERR 37
; ISR_NOERR 38
; ISR_NOERR 39
; ISR_NOERR 40
; ISR_NOERR 41
; ISR_NOERR 42
; ISR_NOERR 43
; ISR_NOERR 44
; ISR_NOERR 45
; ISR_NOERR 46
; ISR_NOERR 47


%macro interrupt_array_entry 1
dd isr%1
%endmacro

global interrupt_pointer_table
interrupt_pointer_table:
    %assign i 0
    %rep 47
        interrupt_array_entry i
    %assign i i+1
    %endrep