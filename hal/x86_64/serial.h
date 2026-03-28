/* hal/x86_64/serial.h - COM1 UART driver
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_X86_64_SERIAL_H
#define HAL_X86_64_SERIAL_H

void serial_init(void);
void serial_putc(char c);
void serial_puts(const char *s);

#endif /* HAL_X86_64_SERIAL_H */
