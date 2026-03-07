SRC_DIR = ./src
BUILD_DIR = ./build

INCLUDE= -I./src/
CFLAGS=	$(INCLUDE) -g -std=gnu99 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -Wall -Werror -O0
LDFLAGS= -T linker.ld
ASMFLAGS= -f elf32 -g

FILES= 	$(BUILD_DIR)/kernel.asm.o $(BUILD_DIR)/kernel.o \
		$(BUILD_DIR)/drivers/screen/screen.o \
		$(BUILD_DIR)/idt/idt.asm.o $(BUILD_DIR)/idt/idt.o 

KERNEL_SIZE := $(shell stat -c%s build/kernel.bin)
KERNEL_SECTORS := $$(($$KERNEL_SIZE / 512 + 1))

all: build_path $(BUILD_DIR)/disk.img

build_path:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/drivers
	mkdir -p $(BUILD_DIR)/drivers/screen
	mkdir -p $(BUILD_DIR)/idt

# -------------------------
# Bootloader (flat binary)
# -------------------------
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	nasm -f bin $< -o $@

# -------------------------
# Kernel objects
# -------------------------
$(BUILD_DIR)/kernel.asm.o: $(SRC_DIR)/kernel.asm
	nasm $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/screen/screen.o: $(SRC_DIR)/drivers/screen/screen.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt/idt.o: $(SRC_DIR)/idt/idt.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt/idt.asm.o: $(SRC_DIR)/idt/idt.asm
	nasm $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/idt/idt.o: $(SRC_DIR)/idt/idt.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# -------------------------
# Link kernel ELF
# -------------------------
$(BUILD_DIR)/kernel.elf: $(FILES)
	i686-elf-ld $(LDFLAGS) $^ -o $@

# Convert ELF → flat binary
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	i686-elf-objcopy -O binary $< $@


# -------------------------
# Create disk image
# -------------------------
$(BUILD_DIR)/disk.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BUILD_DIR)/boot.bin of=$@ conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$@ seek=1 conv=notrunc

# run final image in qemu 
run: $(BUILD_DIR)/disk.img
	qemu-system-i386 -drive format=raw,file=$< -nographic

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run build_path