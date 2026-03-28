/* vm/vm_map.h - Mach vm_map structure
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * vm_map is the per-task address space descriptor. Populated in Phase 2
 * when the virtual memory manager is implemented.
 */

#ifndef VM_VM_MAP_H
#define VM_VM_MAP_H

#include <vm/vm_types.h>

struct vm_map {
    vm_address_t    min_offset;
    vm_address_t    max_offset;
    uint32_t        ref_count;
    uint32_t        nentries;
};

#endif /* VM_VM_MAP_H */
