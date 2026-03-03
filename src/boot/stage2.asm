[BITS 16]
ORG 0x8000

CODE_SEG            equ gdt_code - gdt_start
DATA_SEG            equ gdt_data - gdt_start

KERNEL_LOAD_ADDR    equ 0x00100000
KERNEL_LBA          equ 2
KERNEL_SECTORS      equ 6

ATA_DATA_PORT            equ 0x1F0
ATA_SEC_COUNT_PORT       equ 0x1F2
ATA_LBA_LOW_PORT         equ 0x1F3
ATA_LBA_MID_PORT         equ 0x1F4
ATA_LBA_HIGH_PORT        equ 0x1F5
ATA_DRIVE_PORT           equ 0x1F6
ATA_STATUS_PORT          equ 0x1F7
ATA_COMMAND_PORT         equ 0x1F7

ATA_CMD_READ        equ 0x20


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
    dw gdt_end - gdt_start - 1  ; size of gdt in bytes, -1 because we are not including the last byte (gdt_end)
    dd gdt_start                ; start address of gdt



; ============================
; 32-bit Protected Mode Code
; ============================

[BITS 32]
protected_mode_entry:

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    call load_kernel
    jmp KERNEL_LOAD_ADDR

; ----------------------------
; Load Kernel (6 sectors)
; ----------------------------
global load_kernel

load_kernel:

    mov esi, KERNEL_LBA        ; current LBA
    mov edi, KERNEL_LOAD_ADDR  ; memory destination
    mov ecx, KERNEL_SECTORS    ; sector count

.read_sector:

    push ecx

.wait_not_busy:
    mov dx, ATA_STATUS_PORT
    in al, dx
    test al, 0x80              ; BSY
    jnz .wait_not_busy

    ; -------------------------
    ; Setup read for 1 sector
    ; -------------------------
    mov dx, ATA_SEC_COUNT_PORT
    mov al, 1
    out dx, al

    ; --- LBA bits 0-7 ---
    mov eax, esi
    mov dx, ATA_LBA_LOW_PORT
    out dx, al

    ; --- LBA bits 8-15 ---
    mov eax, esi
    shr eax, 8
    mov dx, ATA_LBA_MID_PORT
    out dx, al

    ; --- LBA bits 16-23 ---
    mov eax, esi
    shr eax, 16
    mov dx, ATA_LBA_HIGH_PORT
    out dx, al

    ; --- LBA bits 24-27 ---
    mov eax, esi
    shr eax, 24
    and al, 0x0F
    or  al, 0xE0               ; 1110xxxx (LBA mode, master)
    mov dx, ATA_DRIVE_PORT
    out dx, al

    ; Send READ command
    mov dx, ATA_COMMAND_PORT
    mov al, ATA_CMD_READ
    out dx, al

.wait_drq:
    in al, dx
    test al, 0x80      ; BSY
    jnz .wait_drq

    test al, 0x01      ; ERR
    jnz .disk_error

    test al, 0x08      ; DRQ
    jz .wait_drq

    ; -------------------------
    ; Read 512 bytes
    ; -------------------------

    mov dx, ATA_DATA_PORT
    mov ecx, 256
    cld                 ; ensure forward direction
    rep insw

    pop ecx
    inc esi
    loop .read_sector

    ret

.disk_error:
    cli
.hang:
    hlt
    jmp .hang
