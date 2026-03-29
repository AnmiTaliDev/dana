/* hal/x86_64/gdt.c - Global Descriptor Table setup
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <hal/x86_64/gdt.h>
#include <libkern/string.h>
#include <stdint.h>

struct gdt_entry {
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_hi;
} __attribute__((packed));

struct gdtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* 3 standard + 2 for the 16-byte TSS descriptor. */
static struct gdt_entry gdt[5];
static struct tss        kernel_tss;

static void gdt_set(int i, uint32_t base, uint32_t limit,
                    uint8_t access, uint8_t gran)
{
    gdt[i].base_lo     = base & 0xffff;
    gdt[i].base_mid    = (base >> 16) & 0xff;
    gdt[i].base_hi     = (base >> 24) & 0xff;
    gdt[i].limit_lo    = limit & 0xffff;
    gdt[i].granularity = ((limit >> 16) & 0x0f) | (gran & 0xf0);
    gdt[i].access      = access;
}

static void gdt_set_tss(int i, uint64_t base, uint32_t limit)
{
    gdt[i].limit_lo    = limit & 0xffff;
    gdt[i].base_lo     = base & 0xffff;
    gdt[i].base_mid    = (base >> 16) & 0xff;
    gdt[i].access      = 0x89;                  /* present, 64-bit available TSS */
    gdt[i].granularity = (limit >> 16) & 0x0f;
    gdt[i].base_hi     = (base >> 24) & 0xff;

    uint32_t *hi = (uint32_t *)&gdt[i + 1];
    hi[0] = (uint32_t)(base >> 32);
    hi[1] = 0;
}

void gdt_init(void)
{
    gdt_set(0, 0, 0,       0x00, 0x00);  /* null */
    gdt_set(1, 0, 0xfffff, 0x9a, 0xa0);  /* kernel code: 64-bit, DPL0 */
    gdt_set(2, 0, 0xfffff, 0x92, 0xa0);  /* kernel data: 64-bit, DPL0 */

    struct gdtr gdtr = {
        .limit = sizeof(gdt) - 1,
        .base  = (uint64_t)&gdt,
    };

    __asm__ volatile (
        "lgdt %0\n"
        "push %1\n"
        "lea 1f(%%rip), %%rax\n"
        "push %%rax\n"
        "lretq\n"
        "1:\n"
        "mov %2, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        : "m"(gdtr),
          "i"((uint64_t)GDT_KERNEL_CODE),
          "i"((uint16_t)GDT_KERNEL_DATA)
        : "rax", "memory"
    );
}

void gdt_load_tss(void)
{
    kmemset(&kernel_tss, 0, sizeof(kernel_tss));
    kernel_tss.iomap_base = sizeof(kernel_tss);

    gdt_set_tss(3, (uint64_t)&kernel_tss, sizeof(kernel_tss) - 1);

    __asm__ volatile ("ltr %0" : : "r"((uint16_t)GDT_TSS));
}

void gdt_set_rsp0(uint64_t rsp0)
{
    kernel_tss.rsp0 = rsp0;
}
