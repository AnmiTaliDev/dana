/* hal/hal.h - Hardware Abstraction Layer interface
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This is the sole contract between the platform-independent kernel and
 * any architecture-specific implementation. Adding a new HAL means
 * providing a hal/<arch>/ directory that implements every function declared
 * here, with no changes to kern/ or vm/.
 */

#ifndef HAL_HAL_H
#define HAL_HAL_H

#include <stdint.h>
#include <stddef.h>

void hal_early_console_init(void);
void hal_console_putc(char c);
void hal_console_set_framebuffer(uint64_t addr, uint32_t width, uint32_t height,
                                  uint32_t pitch, uint8_t bpp);
void hal_init(void);
void hal_halt(void) __attribute__((noreturn));

#endif /* HAL_HAL_H */
