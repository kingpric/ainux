BITS 16
ORG 0x8000

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

start_stage2:
    cli

; ----------------------------
; Enable A20 (Fast method)
; ----------------------------
    in al, 0x92
    or al, 00000010b
    out 0x92, al

; ----------------------------
; Load GDT
; ----------------------------
    lgdt [gdt_descriptor]

; ----------------------------
; Enable Protected Mode
; ----------------------------
    mov eax, cr0
    or eax, 1
    mov cr0, eax

; Far jump to flush pipeline
    jmp CODE_SEG:protected_mode_entry   ; 0x08 is the code segment selector

; ============================
; GDT Definition
; https://wiki.osdev.org/Global_Descriptor_Table
; ============================


gdt_start:

gdt_null:
    dq 0x0000000000000000


; 00 CF 9A 00 0000 FFFF
gdt_code:
    dw 0xFFFF     ; Limit (0-15)
    dw 0x0000     ; Base (0-15)
    db 0x00       ; Base (16-23)
    db 10011010b  ; Access byte (Present, Ring 0, Code, Executable, Readable)
    db 11001111b  ; Flags (Granularity 4KB, 32-bit, Limit 16-19)
    db 0x00       ; Base (24-31)    
    

; 00 CF 92 00 0000 FFFF
gdt_data:
    dw 0xFFFF     ; Limit (0-15)
    dw 0x0000     ; Base (0-15)
    db 0x00       ; Base (16-23)
    db 10010010b  ; Access byte (Present, Ring 0, Data, Read/Write)
    db 11001111b  ; Flags (Granularity 4KB, 32-bit, Limit 16-19)
    db 0x00       ; Base (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ============================
; 32-bit Protected Mode Code
; ============================

BITS 32

protected_mode_entry:

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    call kernel_entry

halt:
    jmp halt

; ============================
; Kernel Entry
; ============================

kernel_entry:

    mov edi, 0xB8000
    mov esi, kernel_message

print_kernel:
    lodsb
    cmp al, 0
    je done_kernel
    mov ah, 0x07
    stosw
    jmp print_kernel

done_kernel:
    ret


kernel_message db "Ainux Kernel Started!", 0