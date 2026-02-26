# Ainux OS Makefile

BOOT_DIR = ./src/boot
BUILD_DIR = ./build

STAGE1 = $(BOOT_DIR)/stage1.asm
STAGE2 = $(BOOT_DIR)/stage2.asm

STAGE1_BIN = $(BUILD_DIR)/stage1.bin
STAGE2_BIN = $(BUILD_DIR)/stage2.bin

DISK_IMAGE = $(BUILD_DIR)/disk.img

all: $(DISK_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(STAGE1_BIN): $(STAGE1) | $(BUILD_DIR)
	nasm -f bin $< -o $@

$(STAGE2_BIN): $(STAGE2) | $(BUILD_DIR)
	nasm -f bin $< -o $@

$(DISK_IMAGE): $(STAGE1_BIN) $(STAGE2_BIN)
	dd if=/dev/zero of=$(DISK_IMAGE) bs=512 count=2880
	dd if=$(STAGE1_BIN) of=$(DISK_IMAGE) conv=notrunc
	dd if=$(STAGE2_BIN) of=$(DISK_IMAGE) bs=512 seek=1 conv=notrunc

run: $(DISK_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(DISK_IMAGE) -nographic

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean