/* main.c - DANA kernel C entry point
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>
#include <stddef.h>
#include <multiboot2.h>
#include <hal/hal.h>
#include <hal/x86_64/pmap.h>
#include <libkern/printf.h>
#include <kern/task.h>
#include <vm/pmm.h>
#include <vm/vm_map.h>

void kmain(uint32_t magic, uint32_t info_ptr) {
    hal_early_console_init();

    kprintf("DANA: booting...\n");
    kprintf("DANA: DANA is Almost Not Apple\n");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("DANA: ERROR: Invalid Multiboot2 magic: 0x%x\n", magic);
        hal_halt();
    }

    struct multiboot2_info *info = (void *)(uintptr_t)info_ptr;
    struct multiboot2_tag *tag = (void *)(uintptr_t)(info_ptr + 8);

    hal_init();
    kprintf("DANA: GDT and IDT loaded\n");
    kprintf("DANA: Multiboot2 info size = %u\n", info->total_size);

    struct multiboot2_tag_mmap        *mmap_tag = NULL;
    struct multiboot2_tag_framebuffer *fb_tag   = NULL;

    while (tag->type != MULTIBOOT2_TAG_TYPE_END) {
        switch (tag->type) {
            case MULTIBOOT2_TAG_TYPE_BASIC_MEMINFO: {
                struct multiboot2_tag_basic_meminfo *m = (void *)tag;
                kprintf("DANA: lower memory: %u KB, upper memory: %u KB\n",
                        m->mem_lower, m->mem_upper);
                break;
            }
            case MULTIBOOT2_TAG_TYPE_MMAP: {
                mmap_tag = (struct multiboot2_tag_mmap *)tag;
                int entries = (int)((mmap_tag->size - sizeof(*mmap_tag)) / mmap_tag->entry_size);
                kprintf("DANA: memory map has %d entries\n", entries);
                break;
            }
            case MULTIBOOT2_TAG_TYPE_FRAMEBUFFER: {
                fb_tag = (struct multiboot2_tag_framebuffer *)tag;
                break;
            }
        }
        tag = (void *)((uintptr_t)tag + ((tag->size + 7) & ~7));
    }

    if (fb_tag && fb_tag->framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_RGB) {
        hal_console_set_framebuffer(
            fb_tag->framebuffer_addr,
            fb_tag->framebuffer_width,
            fb_tag->framebuffer_height,
            fb_tag->framebuffer_pitch,
            fb_tag->framebuffer_bpp);
        kprintf("DANA: framebuffer %ux%u %ubpp\n",
                fb_tag->framebuffer_width,
                fb_tag->framebuffer_height,
                fb_tag->framebuffer_bpp);
    }

    if (mmap_tag == NULL) {
        kprintf("DANA: ERROR: no memory map from bootloader\n");
        hal_halt();
    }

    pmm_init(mmap_tag);
    pmap_init();
    vm_map_init();

    kprintf("DANA: kernel task id = %u\n", kernel_task.task_id);
    kprintf("DANA: boot complete\n");

    hal_halt();
}
