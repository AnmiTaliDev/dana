; hal/x86_64/boot_entry.S - Multiboot2 handoff entry point
; Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
; SPDX-License-Identifier: GPL-3.0-or-later

[bits 32]

section .multiboot_header
align 8
    dd 0xe85250d6                ; magic (Multiboot2)
    dd 0                         ; architecture (0 = i386)
    dd .header_end - .header_start ; header length
    dd -(0xe85250d6 + 0 + (.header_end - .header_start)) ; checksum
.header_start:
    ; Framebuffer request (optional — GRUB will provide a linear framebuffer if possible)
    align 8
    dw 5    ; type = MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    dw 1    ; flags = optional
    dd 20   ; size
    dd 0    ; preferred width  (0 = no preference)
    dd 0    ; preferred height (0 = no preference)
    dd 32   ; preferred depth  (32 bpp)
    ; End tag
    align 8
    dw 0
    dw 0
    dd 8
.header_end:

section .text.boot
global _start
_start:
    cli
    cld

    ; Save Multiboot2 magic and info pointer
    mov     [boot_magic], eax
    mov     [boot_info], ebx

    ; 1. Check CPUID
    pushfd
    pop     eax
    mov     ecx, eax
    xor     eax, 1 << 21
    push    eax
    popfd
    pushfd
    pop     eax
    push    ecx
    popfd
    xor     eax, ecx
    jz      .no_cpuid

    ; 2. Check Long Mode
    mov     eax, 0x80000000
    cpuid
    cmp     eax, 0x80000001
    jb      .no_long_mode
    mov     eax, 0x80000001
    cpuid
    test    edx, 1 << 29
    jz      .no_long_mode

    ; 3. Page Tables
    mov     edi, temp_pml4
    xor     eax, eax
    mov     ecx, 3072
    rep stosd

    mov     eax, temp_pdpt
    or      eax, 0x3
    mov     [temp_pml4], eax
    mov     [temp_pml4 + 511*8], eax

    mov     eax, temp_pd
    or      eax, 0x3
    mov     [temp_pdpt], eax
    mov     [temp_pdpt + 510*8], eax
    mov     [temp_pdpt + 511*8], eax

    mov     eax, 0x83
    mov     edi, temp_pd
    mov     ecx, 512
.loop_pd:
    mov     [edi], eax
    add     eax, 0x200000
    add     edi, 8
    loop    .loop_pd

    ; 4. Enable PAE
    mov     eax, cr4
    or      eax, 1 << 5
    mov     cr4, eax

    ; 5. CR3
    mov     eax, temp_pml4
    mov     cr3, eax

    ; 6. EFER
    mov     ecx, 0xC0000080
    rdmsr
    or      eax, 1 << 8
    wrmsr

    ; 7. Paging
    mov     eax, cr0
    or      eax, 1 << 31
    mov     cr0, eax

    ; 8. GDT
    lgdt    [gdt64_ptr]
    jmp     0x08:_start64

.no_cpuid:
.no_long_mode:
    mov     dword [0xb8000], 0x4f524f45
    hlt
    jmp     $

[bits 64]
extern kmain
extern _bss_start
extern _bss_end

_start64:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    lea     rsp, [rel _boot_stack_top]
    xor     rbp, rbp

    lea     rdi, [rel _bss_start]
    lea     rcx, [rel _bss_end]
    sub     rcx, rdi
    xor     eax, eax
    rep stosb

    mov     edi, [rel boot_magic]
    mov     esi, [rel boot_info]
    call    kmain

.hang:
    cli
    hlt
    jmp     .hang

section .data.boot
align 4096
temp_pml4: times 4096 db 0
temp_pdpt: times 4096 db 0
temp_pd:   times 4096 db 0

boot_magic: dd 0
boot_info:  dd 0

align 16
gdt64:
    dq 0
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF
gdt64_ptr:
    dw $ - gdt64 - 1
    dq gdt64

section .bss
align 16
_boot_stack_bottom:
    resb    65536
global _boot_stack_top
_boot_stack_top:
