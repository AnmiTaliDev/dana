/* kern/thread.h - Mach thread structure
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * A thread is the unit of CPU execution. The saved CPU state will be
 * fully populated in Phase 3 when a scheduler is implemented.
 */

#ifndef KERN_THREAD_H
#define KERN_THREAD_H

#include <kern/kern_types.h>

struct thread_cpu_state {
    uint64_t rsp, rip, rflags;
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp;
    uint64_t r8,  r9,  r10, r11;
    uint64_t r12, r13, r14, r15;
    uint64_t cs, ss;
};

struct thread {
    uint32_t                thread_id;
    uint32_t                ref_count;
    task_t                  task;
    struct thread          *task_next;
    struct thread_cpu_state saved_state;
    ipc_port_t              ith_self;
    uint64_t                kernel_stack;
};

kern_return_t thread_create(task_t task, thread_t *thread_out);
kern_return_t thread_destroy(thread_t thread);

#endif /* KERN_THREAD_H */
