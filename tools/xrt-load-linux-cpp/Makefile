.PHONY: all clean debug

-include image.mk

WORK_ROOT ?= ../../..
#BOOTROM_BIN ?= $(WORK_ROOT)/VexRiscv/sw/bootrom/bootrom.bin
#BOOTROM_ELF ?= $(WORK_ROOT)/VexRiscv/sw/bootrom/bootrom.elf
BOOTROM_BIN ?= $(WORK_ROOT)/VexRiscv/sw/bootrom-rs/bootrom-rs.bin
BOOTROM_ELF ?= $(WORK_ROOT)/VexRiscv/sw/bootrom-rs/target/riscv32imac-unknown-none-elf/debug/bootrom-rs
DEVICETREE ?= $(WORK_ROOT)/VexRiscv/vivado/alveo_u50_linux/cpu_dts/system-top.dtb
SBI_BIN ?= $(WORK_ROOT)/opensbi/build/platform/vexriscv_litefury/firmware/fw_jump.bin
SBI_ELF ?= $(WORK_ROOT)/opensbi/build/platform/vexriscv_litefury/firmware/fw_jump.elf
LINUX_IMAGE ?= $(WORK_ROOT)/buildroot-2021.11.1/output/images/Image
LINUX_ELF ?= $(WORK_ROOT)/buildroot-2021.11.1/output/build/linux-5.15/vmlinux
ROOTFS_IMAGE ?= $(WORK_ROOT)/buildroot-2021.11.1/output/images/rootfs.cpio.gz
XRT_LOAD_LINUX ?= build/xrt-load-linux

IMAGES_CONTENTS :=  images/bootrom.bin \
					images/bootrom.elf \
					images/devicetree.dtb \
					images/fw_jump.bin \
					images/fw_jump.elf \
					images/Image \
					images/vmlinux \
					images/rootfs.cpio.gz \
					images/image.mk \
					images/xrt-load-linux \
					images/Makefile

all: load

load: $(BOOTROM_BIN) $(DEVICETREE) $(SBI_BIN) $(LINUX_IMAGE) $(ROOTFS_IMAGE) $(XRT_LOAD_LINUX)
	$(XRT_LOAD_LINUX) $(BOOTROM_BIN) $(DEVICETREE) $(SBI_BIN) $(LINUX_IMAGE) $(ROOTFS_IMAGE)

debug:
	riscv64-unknown-elf-gdb $(LINUX_ELF) -ex "add-symbol-file $(SBI_ELF)" -ex "add-symbol-file $(BOOTROM_ELF)" -ex "target extended-remote localhost:3333"

openocd:
	openocd -f ./vexriscv_linux.tcl

build/xrt-load-linux: src/main.cpp
	mkdir -p build
	cd build; cmake .. && make

pack: images $(IMAGES_CONTENTS)
	tar zcf images.tar.gz images

images:
	-$(RM) -r images
	mkdir -p images
images/bootrom.bin: $(BOOTROM_BIN)
	cp $< $@
images/bootrom.elf: $(BOOTROM_ELF)
	cp $< $@
images/devicetree.dtb: $(DEVICETREE)
	cp $< $@
images/fw_jump.bin: $(SBI_BIN)
	cp $< $@
images/fw_jump.elf: $(SBI_ELF)
	cp $< $@
images/Image: $(LINUX_IMAGE)
	cp $< $@
images/vmlinux: $(LINUX_ELF)
	cp $< $@
images/rootfs.cpio.gz: $(ROOTFS_IMAGE)
	cp $< $@
images/image.mk: image.mk.template
	cp $< $@
images/xrt-load-linux: build/xrt-load-linux
	mkdir -p $(dir $@)
	cp $< $@
images/Makefile: Makefile
	cp $< $@

$(BOOTROM_BIN):
	cd $(dir $(BOOTROM_BIN)); make

$(DEVICETREE):
	cd $(dir $(DEVICETREE)); make

clean:
	-$(RM) -r images
	-$(RM) images.tar.gz
	-$(RM) -r build