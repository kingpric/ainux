; boot.asm
; Our first boot sector

BITS 16
ORG 0X7C00

%define OS_NAME "Ainux"

start:
    cli             ; clear interrupt flag

    xor ax,ax       ; setting 0H (0x0000) to AX
    mov ds, ax      ; copying AX to DS (data segmrnt)
    mov es, ax      ; copying AX to ES (extra segment)
    mov ss, ax      ; copying AX to SS (stack segment)
    mov sp, 0x7C00  ; copying 0x7C00 value to SP (stack pointer)

    sti             ; set interrupt flag

    mov [boot_drive], dl   ; Save boot drive

    ; ------------------------
    ; Load Stage 2 from disk
    ; ------------------------

    mov ah, 0x02    ; BIOS Read sectors
    mov al, 1       ; Number of sectors to read
    mov ch, 0       ; Cylinder (0 for single cylinder)
    mov cl, 2       ; Sector (2 for second sector)
    mov dh, 0       ; Head (0 for single head)
    mov dl, [boot_drive]   ; Restore drive number
    mov bx, 0x8000  ; Buffer address
    int 13h         ; Call interrupt

    jc .disk_error   ; Jump if carry flag is set

    jmp 0x0000:0x8000      ; Jump to Stage 2

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


boot_drive db 0
disk_error_msg db "Disk Read Error!", 0
welcome_msg db "Welcome to ", OS_NAME, " - Stage 1", 0

times 510-($-$$) db 0   ; set remaining bytes to 0 upto 510 address
dw 0xaa55           ; boot magic number