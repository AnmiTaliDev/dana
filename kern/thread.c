/* kern/thread.c - Mach thread management stubs
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <kern/thread.h>

kern_return_t thread_create(task_t task, thread_t *thread_out) {
    (void)task;
    (void)thread_out;
    return KERN_NOT_SUPPORTED;
}

kern_return_t thread_destroy(thread_t thread) {
    (void)thread;
    return KERN_NOT_SUPPORTED;
}
