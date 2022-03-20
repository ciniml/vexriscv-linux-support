.PHONY: all clean debug

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

all: load

load: $(BOOTROM_BIN) $(DEVICETREE) $(SBI_BIN) $(LINUX_IMAGE) $(ROOTFS_IMAGE) build/xrt-load-linux
	build/xrt-load-linux $(BOOTROM_BIN) $(DEVICETREE) $(SBI_BIN) $(LINUX_IMAGE) $(ROOTFS_IMAGE)

debug:
	riscv64-unknown-elf-gdb $(LINUX_ELF) -ex "add-symbol-file $(SBI_ELF)" -ex "add-symbol-file $(BOOTROM_ELF)" -ex "target extended-remote localhost:3333"

openocd:
	openocd -f ./vexriscv_linux.tcl

build/xrt-load-linux: src/main.cpp
	mkdir -p build
	cd build; cmake .. && make

$(BOOTROM_BIN):
	cd $(dir $(BOOTROM_BIN)); make

$(DEVICETREE):
	cd $(dir $(DEVICETREE)); make
