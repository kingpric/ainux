# Ainux OS Makefile

# Directories
BASE_DIR = .
SRC_DIR = $(BASE_DIR)/src
BOOT_DIR = $(SRC_DIR)/boot
BUILD_DIR = $(BASE_DIR)/build

# Files
STAGE1 = $(BOOT_DIR)/stage1.asm
STAGE2 = $(BOOT_DIR)/stage2.asm

STAGE1_BIN = $(BUILD_DIR)/stage1.bin
STAGE2_BIN = $(BUILD_DIR)/stage2.bin

DISK_IMAGE = $(BUILD_DIR)/disk.img

all: $(DISK_IMAGE)

# Rules
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# create stage1 binary
$(STAGE1_BIN): $(STAGE1) | $(BUILD_DIR)
	nasm -f bin $< -o $@

# create stage2 binary
$(STAGE2_BIN): $(STAGE2) | $(BUILD_DIR)
	nasm -f bin $< -o $@

# create disk image
$(DISK_IMAGE): $(STAGE1_BIN) $(STAGE2_BIN)
	dd if=/dev/zero of=$(DISK_IMAGE) bs=512 count=2880
	dd if=$(STAGE1_BIN) of=$(DISK_IMAGE) conv=notrunc
	dd if=$(STAGE2_BIN) of=$(DISK_IMAGE) bs=512 seek=1 conv=notrunc

# run final image in qemu 
run: $(DISK_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(DISK_IMAGE) -nographic

# clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean