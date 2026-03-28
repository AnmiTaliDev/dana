; hal/x86_64/boot_entry.S - Limine handoff entry point
; Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
; SPDX-License-Identifier: GPL-3.0-or-later
;
; Limine places us in 64-bit long mode with paging enabled and the kernel
; mapped at the higher-half address. Interrupts are disabled. We must:
;   1. Set up our own stack (Limine's stack is temporary and small)
;   2. Zero the BSS region
;   3. Call kmain()
;   4. Halt if kmain returns (it should not)

[bits 64]
[section .text.boot]

extern kmain
extern _bss_start
extern _bss_end

global _start
_start:
    lea     rsp, [rel _boot_stack_top]
    xor     rbp, rbp

    lea     rdi, [rel _bss_start]
    lea     rcx, [rel _bss_end]
    sub     rcx, rdi
    xor     eax, eax
    rep stosb

    call    kmain

.hang:
    cli
    hlt
    jmp     .hang

[section .bss]
align 16
_boot_stack_bottom:
    resb    65536
_boot_stack_top:
