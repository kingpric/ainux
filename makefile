SRC_DIR = ./src
BUILD_DIR = ./build

all: $(BUILD_DIR)/disk.img

build_path:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.bin:$(SRC_DIR)/boot/boot.asm | build_path
	nasm -f bin $< -o $@

$(BUILD_DIR)/kernel.bin:$(SRC_DIR)/kernel.asm | build_path
	nasm -f bin $< -o $@

$(BUILD_DIR)/disk.img:$(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BUILD_DIR)/boot.bin of=$@ conv=notrunc
	dd if=$(BUILD_DIR)/kernel.bin of=$@ seek=1 conv=notrunc

# run final image in qemu 
run: $(BUILD_DIR)/disk.img
	qemu-system-i386 -drive format=raw,file=$< -nographic

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all