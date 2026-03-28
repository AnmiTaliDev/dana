/* vm/pmm.c - Physical memory manager (bitmap allocator)
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * One bit per 4 KB page; 0 = free, 1 = used.
 * The bitmap is placed in the first USABLE region large enough to hold it.
 * With Limine, LIMINE_MEMMAP_USABLE regions never overlap the kernel,
 * bootloader data, or framebuffer — no manual kernel reservation needed.
 */

#include <stddef.h>
#include <stdint.h>
#include <limine.h>
#include <vm/pmm.h>
#include <hal/pmap.h>
#include <libkern/string.h>
#include <libkern/printf.h>

#define BITS_PER_WORD   64UL
#define WORD_IDX(page)  ((page) / BITS_PER_WORD)
#define BIT_IDX(page)   ((page) % BITS_PER_WORD)

/* Track up to 4 GB (1M pages). */
#define PMM_MAX_PAGES    (1UL << 20)
#define PMM_BITMAP_WORDS (PMM_MAX_PAGES / BITS_PER_WORD)
#define PMM_BITMAP_BYTES (PMM_BITMAP_WORDS * sizeof(uint64_t))

static uint64_t *bitmap;
static size_t    total_pages;
static size_t    free_count;

static void bitmap_set(size_t page)
{
    bitmap[WORD_IDX(page)] |= (1ULL << BIT_IDX(page));
}

static void bitmap_clear(size_t page)
{
    bitmap[WORD_IDX(page)] &= ~(1ULL << BIT_IDX(page));
}

static int bitmap_test(size_t page)
{
    return (bitmap[WORD_IDX(page)] >> BIT_IDX(page)) & 1;
}

void pmm_init(struct limine_memmap_response *mmap,
              uint64_t kern_phys_start, uint64_t kern_phys_end)
{
    /* Find the first USABLE region above 1 MB that fits the bitmap. */
    uint64_t bitmap_phys = 0;
    for (uint64_t i = 0; i < mmap->entry_count; i++) {
        struct limine_memmap_entry *e = mmap->entries[i];
        if (e->type != LIMINE_MEMMAP_USABLE)
            continue;
        uint64_t start = (e->base + PMM_PAGE_SIZE - 1) & ~(PMM_PAGE_SIZE - 1);
        if (start < 0x100000)
            start = 0x100000;
        if (start + PMM_BITMAP_BYTES > e->base + e->length)
            continue;
        bitmap_phys = start;
        break;
    }

    if (bitmap_phys == 0) {
        kprintf("PMM: FATAL: no region for bitmap\n");
        return;
    }

    bitmap = (uint64_t *)PHYS_TO_VIRT(bitmap_phys);

    /* Mark everything used. */
    kmemset(bitmap, 0xff, PMM_BITMAP_BYTES);
    total_pages = 0;
    free_count  = 0;

    /* Free all USABLE pages. */
    for (uint64_t i = 0; i < mmap->entry_count; i++) {
        struct limine_memmap_entry *e = mmap->entries[i];
        if (e->type != LIMINE_MEMMAP_USABLE)
            continue;

        uint64_t start = (e->base + PMM_PAGE_SIZE - 1) & ~(PMM_PAGE_SIZE - 1);
        uint64_t end   = (e->base + e->length) & ~(PMM_PAGE_SIZE - 1);

        for (uint64_t addr = start; addr < end; addr += PMM_PAGE_SIZE) {
            size_t page = (size_t)(addr >> PMM_PAGE_SHIFT);
            if (page >= PMM_MAX_PAGES)
                break;
            total_pages++;
            bitmap_clear(page);
            free_count++;
        }
    }

    /* Reserve page 0. */
    if (!bitmap_test(0)) { bitmap_set(0); free_count--; }

    /* Reserve the bitmap itself. */
    uint64_t bm_end = bitmap_phys + PMM_BITMAP_BYTES;
    for (uint64_t addr = bitmap_phys; addr < bm_end; addr += PMM_PAGE_SIZE) {
        size_t page = (size_t)(addr >> PMM_PAGE_SHIFT);
        if (page < PMM_MAX_PAGES && !bitmap_test(page)) {
            bitmap_set(page);
            free_count--;
        }
    }

    /* Reserve kernel image pages (extra safety in case Limine didn't mark them). */
    for (uint64_t addr = kern_phys_start; addr < kern_phys_end; addr += PMM_PAGE_SIZE) {
        size_t page = (size_t)(addr >> PMM_PAGE_SHIFT);
        if (page < PMM_MAX_PAGES && !bitmap_test(page)) {
            bitmap_set(page);
            free_count--;
        }
    }

    kprintf("PMM: %u MB free (%u total pages)\n",
            (unsigned)(free_count >> 8),
            (unsigned)total_pages);
}

uint64_t pmm_alloc_page(void)
{
    for (size_t w = 0; w < PMM_BITMAP_WORDS; w++) {
        if (bitmap[w] == ~0ULL)
            continue;
        int bit = __builtin_ctzll(~bitmap[w]);
        size_t page = w * BITS_PER_WORD + (size_t)bit;
        if (page >= PMM_MAX_PAGES)
            break;
        bitmap_set(page);
        if (free_count > 0) free_count--;
        return (uint64_t)page << PMM_PAGE_SHIFT;
    }
    return PMM_NULL;
}

void pmm_free_page(uint64_t paddr)
{
    size_t page = (size_t)(paddr >> PMM_PAGE_SHIFT);
    if (page == 0 || page >= PMM_MAX_PAGES)
        return;
    bitmap_clear(page);
    free_count++;
}

size_t pmm_free_pages(void)  { return free_count; }
size_t pmm_total_pages(void) { return total_pages; }
