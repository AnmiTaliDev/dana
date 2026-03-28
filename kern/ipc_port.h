/* kern/ipc_port.h - Mach IPC port structure
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Ports are the fundamental communication primitive in Mach. A port is a
 * protected message queue; send and receive rights are capabilities that
 * refer to a port. Mirrors XNU's osfmk/ipc/ipc_port.h at a high level.
 */

#ifndef KERN_IPC_PORT_H
#define KERN_IPC_PORT_H

#include <kern/kern_types.h>

typedef enum {
    IPC_PORT_TYPE_NONE    = 0,
    IPC_PORT_TYPE_SEND    = 1,
    IPC_PORT_TYPE_RECEIVE = 2,
    IPC_PORT_TYPE_DEAD    = 3,
} ipc_port_type_t;

struct ipc_port {
    mach_port_name_t  ip_name;
    ipc_port_type_t   ip_type;
    uint32_t          ip_ref_count;
    uint32_t          ip_mscount;
    uint32_t          ip_srights;
    void             *ip_kobject;
};

kern_return_t ipc_port_alloc(ipc_port_t *port_out);
void          ipc_port_release(ipc_port_t port);

#endif /* KERN_IPC_PORT_H */
