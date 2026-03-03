BITS 16
ORG 0x8000

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


; ----------------------------
; Get Boot Drive
; ----------------------------
mov [boot_drive], dl

start_stage2:
    cli

; ----------------------------
; Enable A20 (Fast method)
; ----------------------------
    in al, 0x92
    or al, 00000010b
    out 0x92, al

; ----------------------------
; Load Kernel (6 sectors)
; ----------------------------
    mov ax, 0x9000
    mov es, ax
    mov bx, 0x0000        ; ES:BX = 0x90000

    mov ah, 0x02          ; BIOS read
    mov al, 6             ; sectors to read
    mov ch, 0
    mov cl, 3             ; start from sector 3
    mov dh, 0
    mov dl, [boot_drive]
    sti
    int 0x13    
    cli

    jc .disk_error   ; Jump if carry flag is set
    jmp load_gdt

.disk_error:
    mov si, disk_error_msg ; set SI (Source index) to start location of message
    jmp .print

.print:
    lodsb           ; load string byte to al from [si] and increament si = si + 1 
    cmp al, 0       ; compare al to 0 (null)
    je done         ; if compare is equal then jmp to done label

    mov ah, 0x0E     ; mov value 0xoe to ah
    int 10h         ; call interrupt
    jmp .print      ; unconditional jmp to print label

done:               
    jmp $           ; unconditional jmp to here only   

         
; ----------------------------
; Load GDT
; ----------------------------
load_gdt:
    lgdt [gdt_descriptor]

; ----------------------------
; Enable Protected Mode
; ----------------------------
enable_pm:
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

    jmp 0x90000
    
halt:
    jmp halt

boot_drive db 0
disk_error_msg db "Disk Read Error!", 0