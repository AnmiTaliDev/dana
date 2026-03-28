/* main.c - DANA kernel C entry point
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * kmain() is called from hal/x86_64/boot_entry.S after the BSS is zeroed
 * and a boot stack is established. At entry, Limine has already set up
 * 4-level paging and placed us in 64-bit long mode.
 */

#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <hal/hal.h>
#include <libkern/printf.h>
#include <kern/task.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

void kmain(void) {
    hal_early_console_init();

    kprintf("DANA: booting...\n");
    kprintf("DANA: DANA is Almost Not Apple\n");

    hal_init();
    kprintf("DANA: GDT and IDT loaded\n");

    if (memmap_request.response != NULL) {
        uint64_t count = memmap_request.response->entry_count;
        kprintf("DANA: memory map has %lu entries\n", (unsigned long)count);
    }

    if (hhdm_request.response != NULL) {
        kprintf("DANA: HHDM offset = 0x%lx\n",
                (unsigned long)hhdm_request.response->offset);
    }

    kprintf("DANA: kernel task id = %u\n", kernel_task.task_id);
    kprintf("DANA: boot complete\n");

    hal_halt();
}
