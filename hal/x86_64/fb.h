/* hal/x86_64/fb.h - Linear framebuffer console driver
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_X86_64_FB_H
#define HAL_X86_64_FB_H

#include <stdint.h>

void fb_init(uint64_t addr, uint32_t width, uint32_t height,
             uint32_t pitch, uint8_t bpp);
void fb_putc(char c);
int  fb_active(void);

#endif /* HAL_X86_64_FB_H */
