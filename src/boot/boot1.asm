[BITS 16]
ORG 0x7c00

STAGE2_LOAD_ADDR  equ 0x8000
STAGE2_START_SECTOR equ 2
STAGE2_SECTORS    equ 20

.start:
    cli
    mov [drive_number], dl
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00


; Load boot2.bin from disk to memory
.load_boot2:
    mov ah, 0x02                ; read from disk
    mov al, STAGE2_SECTORS      ; number of sectors to read
    mov ch, 0                   ; cylinder number
    mov cl, STAGE2_START_SECTOR ; sector number
    mov dh, 0                   ; head number
    mov dl, [drive_number]      ; drive number
    mov bx, STAGE2_LOAD_ADDR    ; load to memory at 0x1000
    int 0x13
    jc disk_error

    ; print message
    mov si, message
    call print_string

    jmp STAGE2_LOAD_ADDR

disk_error:
    mov si, disk_msg
    call print_string
    jmp $

%include "src/boot/functions.asm"

message db "Booting Ainux...",13,10,0
disk_msg db "Disk error!",13,10,0

drive_number db 0

; Boot sector magic number
times 510 - ($ - $$) db 0
db 0x55, 0xAA