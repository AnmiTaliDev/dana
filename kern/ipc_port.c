/* kern/ipc_port.c - Mach IPC port stubs
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <kern/ipc_port.h>

kern_return_t ipc_port_alloc(ipc_port_t *port_out) {
    (void)port_out;
    return KERN_NOT_SUPPORTED;
}

void ipc_port_release(ipc_port_t port) {
    (void)port;
}
