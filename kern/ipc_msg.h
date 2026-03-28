/* kern/ipc_msg.h - Mach message header
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * mach_msg_header_t layout is ABI-compatible with Darwin's <mach/message.h>.
 * This is intentional: DANA aims to run Darwin userspace binaries that pass
 * real Mach messages.
 */

#ifndef KERN_IPC_MSG_H
#define KERN_IPC_MSG_H

#include <kern/kern_types.h>

typedef uint32_t mach_msg_bits_t;
typedef uint32_t mach_msg_size_t;
typedef int32_t  mach_msg_id_t;
typedef uint32_t mach_msg_type_name_t;

#define MACH_MSGH_BITS_REMOTE_MASK   0x0000001fu
#define MACH_MSGH_BITS_LOCAL_MASK    0x00001f00u
#define MACH_MSGH_BITS_COMPLEX       0x80000000u

#define MACH_MSGH_BITS(remote, local) \
    (((remote) & MACH_MSGH_BITS_REMOTE_MASK) | \
     (((local) << 8) & MACH_MSGH_BITS_LOCAL_MASK))

typedef struct {
    mach_msg_bits_t   msgh_bits;
    mach_msg_size_t   msgh_size;
    mach_port_t       msgh_remote_port;
    mach_port_t       msgh_local_port;
    mach_port_name_t  msgh_voucher_port;
    mach_msg_id_t     msgh_id;
} mach_msg_header_t;

#endif /* KERN_IPC_MSG_H */
