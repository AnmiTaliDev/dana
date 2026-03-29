/* hal/x86_64/gdt.h - Global Descriptor Table
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_X86_64_GDT_H
#define HAL_X86_64_GDT_H

#include <stdint.h>

#define GDT_KERNEL_CODE  0x08
#define GDT_KERNEL_DATA  0x10
#define GDT_TSS          0x18

struct tss {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed));

void     gdt_init(void);
void     gdt_load_tss(void);
void     gdt_set_rsp0(uint64_t rsp0);

#endif /* HAL_X86_64_GDT_H */
