ARCH = x86

SRC_DIR = ./src
BUILD_DIR = ./build
SRC_KERNEL_ARCH_DIR = $(SRC_DIR)/kernel/arch/$(ARCH)
BUILD_KERNEL_ARCH_DIR = $(BUILD_DIR)/kernel/arch/$(ARCH)

INCLUDE= -I./src/
CFLAGS=	$(INCLUDE) -g -std=gnu99 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -Wall -Werror -O0
LDFLAGS= -T linker.ld
ASMFLAGS= -f elf32 -g

FILES= 	$(BUILD_DIR)/kernel/kernel.o $(BUILD_KERNEL_ARCH_DIR)/kernel.asm.o \
		$(BUILD_DIR)/drivers/screen/screen.o \


all: $(BUILD_DIR)/disk.img

# -------------------------
# Bootloader (flat binary)	
# -------------------------
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	@mkdir -p $(@D)
	nasm -f bin $< -o $@

# -------------------------
# Kernel objects
# -------------------------

# Build kernel.o from kernel.c
$(BUILD_DIR)/kernel/kernel.o: $(SRC_DIR)/kernel/kernel.c
	@mkdir -p $(@D)
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# Build all assembly files in the kernel architecture directory
$(BUILD_KERNEL_ARCH_DIR)/%.asm.o: $(SRC_KERNEL_ARCH_DIR)/%.asm
	@mkdir -p $(@D)
	nasm $(ASMFLAGS) $< -o $@

# Build all C files in the kernel architecture directory
$(BUILD_KERNEL_ARCH_DIR)/%.o: $(SRC_KERNEL_ARCH_DIR)/%.c
	@mkdir -p $(@D)
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# -------------------------
# Drivers objects
# -------------------------
$(BUILD_DIR)/drivers/%.o: $(SRC_DIR)/drivers/%.c
	@mkdir -p $(@D)
	i686-elf-gcc $(CFLAGS) -c $< -o $@

# -------------------------
# Link kernel ELF
# -------------------------
$(BUILD_DIR)/kernel.elf: $(FILES)
	@mkdir -p $(@D)
	i686-elf-ld $(LDFLAGS) $^ -o $@

# Convert ELF → flat binary
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	@mkdir -p $(@D)
	i686-elf-objcopy -O binary $< $@


# -------------------------
# Create disk image
# -------------------------
$(BUILD_DIR)/disk.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	@mkdir -p $(@D)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BUILD_DIR)/boot.bin of=$@ conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$@ seek=1 conv=notrunc

# run final image in qemu 
run: $(BUILD_DIR)/disk.img
	qemu-system-i386 -drive format=raw,file=$< -nographic

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run