/* hal/x86_64/cpu.h - x86_64 CPU primitives
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_X86_64_CPU_H
#define HAL_X86_64_CPU_H

#include <stdint.h>

static inline void cpu_halt(void) {
    __asm__ volatile ("hlt");
}

static inline void cpu_pause(void) {
    __asm__ volatile ("pause");
}

static inline void cpu_disable_interrupts(void) {
    __asm__ volatile ("cli");
}

static inline void cpu_enable_interrupts(void) {
    __asm__ volatile ("sti");
}

static inline uint64_t cpu_read_cr0(void) {
    uint64_t val;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(val));
    return val;
}

static inline uint64_t cpu_read_cr2(void) {
    uint64_t val;
    __asm__ volatile ("mov %%cr2, %0" : "=r"(val));
    return val;
}

static inline uint64_t cpu_read_cr3(void) {
    uint64_t val;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(val));
    return val;
}

static inline void cpu_write_cr3(uint64_t val) {
    __asm__ volatile ("mov %0, %%cr3" : : "r"(val) : "memory");
}

static inline void cpu_outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t cpu_inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void cpu_io_wait(void) {
    cpu_outb(0x80, 0);
}

#endif /* HAL_X86_64_CPU_H */
