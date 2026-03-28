/* libkern/string.c - Kernel string and memory utilities
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <libkern/string.h>

void *kmemset(void *dst, int c, size_t n) {
    unsigned char *p = dst;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return dst;
}

void *kmemcpy(void *dst, const void *src, size_t n) {
    unsigned char       *d = dst;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}

size_t kstrlen(const char *s) {
    size_t n = 0;
    while (*s++) {
        n++;
    }
    return n;
}

int kstrcmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
