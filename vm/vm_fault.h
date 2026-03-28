/* vm/vm_fault.h - Mach page fault resolution
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef VM_VM_FAULT_H
#define VM_VM_FAULT_H

#include <vm/vm_map.h>

/* Fault access type — matches x86_64 error code bits for convenience. */
#define VM_FAULT_READ    0
#define VM_FAULT_WRITE   1
#define VM_FAULT_EXECUTE 2

typedef enum {
    VM_FAULT_SUCCESS = 0,
    VM_FAULT_BAD_ADDRESS,
    VM_FAULT_PROTECTION,
    VM_FAULT_OOM,
} vm_fault_return_t;

vm_fault_return_t vm_fault(vm_map_t map, vm_address_t addr, int fault_type);

#endif /* VM_VM_FAULT_H */
