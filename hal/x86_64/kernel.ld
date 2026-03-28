/* hal/x86_64/kernel.ld - DANA x86_64 linker script
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * The kernel is linked at 0xffffffff80000000 (-2GB), which is the
 * conventional higher-half base for the x86_64 kernel code model.
 * Limine maps the kernel there via its internal page tables before
 * transferring control to _start.
 */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

SECTIONS {
    . = 0xffffffff80000000;

    .limine_requests ALIGN(8) : {
        KEEP(*(.limine_requests))
    }

    .text ALIGN(4K) : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata .rodata.*)
    }

    .data ALIGN(4K) : {
        *(.data .data.*)
    }

    .bss ALIGN(4K) : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    }

    _kernel_end = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}
