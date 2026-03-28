/* libkern/printf.c - Kernel formatted output
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Supports: %c %s %d %u %x %p %%. Width and precision not yet implemented.
 */

#include <libkern/printf.h>
#include <libkern/string.h>
#include <hal/hal.h>

static void print_str(const char *s) {
    if (!s) {
        s = "(null)";
    }
    while (*s) {
        hal_console_putc(*s++);
    }
}

static void print_uint(unsigned long long val, int base, int upper) {
    const char *digits_lo = "0123456789abcdef";
    const char *digits_hi = "0123456789ABCDEF";
    const char *digits = upper ? digits_hi : digits_lo;
    char buf[64];
    int i = 0;

    if (val == 0) {
        hal_console_putc('0');
        return;
    }
    while (val) {
        buf[i++] = digits[val % (unsigned)base];
        val /= (unsigned)base;
    }
    while (i--) {
        hal_console_putc(buf[i]);
    }
}

static void print_int(long long val) {
    if (val < 0) {
        hal_console_putc('-');
        print_uint((unsigned long long)-val, 10, 0);
    } else {
        print_uint((unsigned long long)val, 10, 0);
    }
}

void kvprintf(const char *fmt, va_list ap) {
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            hal_console_putc(*fmt);
            continue;
        }
        fmt++;
        switch (*fmt) {
        case 'c':
            hal_console_putc((char)va_arg(ap, int));
            break;
        case 's':
            print_str(va_arg(ap, const char *));
            break;
        case 'd':
        case 'i':
            print_int(va_arg(ap, int));
            break;
        case 'u':
            print_uint(va_arg(ap, unsigned int), 10, 0);
            break;
        case 'x':
            print_uint(va_arg(ap, unsigned int), 16, 0);
            break;
        case 'X':
            print_uint(va_arg(ap, unsigned int), 16, 1);
            break;
        case 'p':
            print_str("0x");
            print_uint((unsigned long long)(uintptr_t)va_arg(ap, void *), 16, 0);
            break;
        case 'l':
            fmt++;
            if (*fmt == 'l') {
                fmt++;
                if (*fmt == 'd' || *fmt == 'i') {
                    print_int(va_arg(ap, long long));
                } else if (*fmt == 'u') {
                    print_uint(va_arg(ap, unsigned long long), 10, 0);
                } else if (*fmt == 'x') {
                    print_uint(va_arg(ap, unsigned long long), 16, 0);
                }
            } else if (*fmt == 'd' || *fmt == 'i') {
                print_int(va_arg(ap, long));
            } else if (*fmt == 'u') {
                print_uint(va_arg(ap, unsigned long), 10, 0);
            } else if (*fmt == 'x') {
                print_uint(va_arg(ap, unsigned long), 16, 0);
            }
            break;
        case '%':
            hal_console_putc('%');
            break;
        default:
            hal_console_putc('%');
            hal_console_putc(*fmt);
            break;
        }
    }
}

void kprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    kvprintf(fmt, ap);
    va_end(ap);
}
