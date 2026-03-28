/* libkern/string.h - Kernel string and memory utilities
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIBKERN_STRING_H
#define LIBKERN_STRING_H

#include <stddef.h>
#include <stdint.h>

void  *kmemset(void *dst, int c, size_t n);
void  *kmemcpy(void *dst, const void *src, size_t n);
size_t kstrlen(const char *s);
int    kstrcmp(const char *a, const char *b);

#endif /* LIBKERN_STRING_H */
