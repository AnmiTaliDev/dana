# GNUmakefile - DANA kernel build system
# Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Usage:
#   make            - build the kernel ELF
#   make iso        - build a bootable ISO image
#   make run        - run in QEMU (requires make iso first)
#   make get-deps   - download include/limine.h
#   make clean      - remove build artifacts

HAL ?= x86_64

CC      := gcc
AS      := nasm
LD      := ld

CFLAGS  := -std=c11 \
           -ffreestanding \
           -fno-stack-protector \
           -fno-stack-check \
           -fno-pie \
           -fno-pic \
           -mno-80387 \
           -mno-mmx \
           -mno-sse \
           -mno-sse2 \
           -mno-red-zone \
           -mcmodel=kernel \
           -Wall \
           -Wextra \
           -Wno-unused-parameter \
           -I. \
           -Iinclude \
           -Ikern \
           -Ivm \
           -Ilibkern \
           -Ihal \
           -Ihal/$(HAL)

ASFLAGS := -f elf64

LDFLAGS := -nostdlib \
           -static \
           -m elf_x86_64 \
           -z max-page-size=0x1000 \
           -T hal/$(HAL)/kernel.ld

BUILD    := build
ISOROOT  := $(BUILD)/isoroot
LIMINE   := /usr/share/limine

SRCS_C  := main.c \
           kern/task.c \
           kern/thread.c \
           kern/ipc_port.c \
           libkern/string.c \
           libkern/printf.c \
           hal/$(HAL)/serial.c \
           hal/$(HAL)/gdt.c \
           hal/$(HAL)/idt.c \
           hal/$(HAL)/hal.c

SRCS_S  := hal/$(HAL)/boot_entry.S

OBJS    := $(patsubst %.c, $(BUILD)/%.o, $(SRCS_C)) \
           $(patsubst %.S, $(BUILD)/%.o, $(SRCS_S))

KERNEL  := $(BUILD)/dana.elf
ISO     := $(BUILD)/dana.iso

.PHONY: all build iso run get-deps clean

all: build

build: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

iso: build
	rm -rf $(ISOROOT)
	mkdir -p $(ISOROOT)/boot/limine $(ISOROOT)/EFI/BOOT
	cp $(KERNEL)                          $(ISOROOT)/boot/dana.elf
	cp hal/$(HAL)/limine.conf             $(ISOROOT)/boot/limine/limine.conf
	cp $(LIMINE)/limine-bios.sys          $(ISOROOT)/boot/limine/
	cp $(LIMINE)/limine-bios-cd.bin       $(ISOROOT)/boot/limine/
	cp $(LIMINE)/limine-uefi-cd.bin       $(ISOROOT)/boot/limine/
	cp $(LIMINE)/BOOTX64.EFI              $(ISOROOT)/EFI/BOOT/
	xorriso -as mkisofs -R -r -J \
	    -b boot/limine/limine-bios-cd.bin \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -hfsplus -apm-block-size 2048 \
	    --efi-boot boot/limine/limine-uefi-cd.bin \
	    -efi-boot-part --efi-boot-image \
	    --protective-msdos-label \
	    $(ISOROOT) -o $(ISO) 2>/dev/null
	limine bios-install $(ISO)

run: iso
	qemu-system-x86_64 \
	    -M q35 \
	    -m 256M \
	    -cdrom $(ISO) \
	    -serial stdio \
	    -no-reboot \
	    -no-shutdown

get-deps:
	@mkdir -p include
	curl -Lo include/limine.h \
	    https://raw.githubusercontent.com/limine-bootloader/limine/v8.x/limine.h

clean:
	rm -rf $(BUILD)
