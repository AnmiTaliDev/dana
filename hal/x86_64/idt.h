/* hal/x86_64/idt.h - Interrupt Descriptor Table
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_X86_64_IDT_H
#define HAL_X86_64_IDT_H

#include <stdint.h>

void idt_init(void);

#endif /* HAL_X86_64_IDT_H */
