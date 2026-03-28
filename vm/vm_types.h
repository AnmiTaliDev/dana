/* vm/vm_types.h - Mach virtual memory types
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Type definitions matching Mach semantics as used in XNU's
 * osfmk/mach/vm_types.h and osfmk/mach/vm_prot.h.
 */

#ifndef VM_VM_TYPES_H
#define VM_VM_TYPES_H

#include <stdint.h>

typedef uint64_t    vm_address_t;
typedef uint64_t    vm_offset_t;
typedef uint64_t    vm_size_t;

typedef int         vm_prot_t;

#define VM_PROT_NONE    0x00
#define VM_PROT_READ    0x01
#define VM_PROT_WRITE   0x02
#define VM_PROT_EXECUTE 0x04
#define VM_PROT_ALL     (VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)
#define VM_PROT_DEFAULT (VM_PROT_READ | VM_PROT_WRITE)

#endif /* VM_VM_TYPES_H */
