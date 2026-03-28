/* hal/x86_64/kernel.ld - DANA x86_64 linker script
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    boot PT_LOAD FLAGS(7); /* RWE */
    text PT_LOAD FLAGS(5); /* R-E */
    data PT_LOAD FLAGS(6); /* RW- */
}

SECTIONS {
    /* Boot section at 1M (physical) */
    . = 0x100000;
    .boot : {
        KEEP(*(.multiboot_header))
        *(.text.boot)
        *(.data.boot)
    } : boot

    /* Kernel linked at higher-half, but loaded after .boot physically */
    . = 0xffffffff80200000;
    _kernel_start_v = .;
    _kernel_start_p = 0x200000;

    .text : AT(_kernel_start_p) {
        *(.text .text.*)
    } : text

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - _kernel_start_v + _kernel_start_p) {
        *(.rodata .rodata.*)
    } : text

    .data ALIGN(4K) : AT(ADDR(.data) - _kernel_start_v + _kernel_start_p) {
        *(.data .data.*)
    } : data

    .bss ALIGN(4K) : AT(ADDR(.bss) - _kernel_start_v + _kernel_start_p) {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    } : data

    _kernel_end = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}
