/* libkern/printf.h - Kernel formatted output
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * kprintf() routes output through hal_console_putc(), keeping this
 * code fully HAL-independent.
 */

#ifndef LIBKERN_PRINTF_H
#define LIBKERN_PRINTF_H

#include <stdarg.h>

void kprintf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void kvprintf(const char *fmt, va_list ap);

#endif /* LIBKERN_PRINTF_H */
