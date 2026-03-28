/* hal/pmap.h - HAL-independent pmap interface selector
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Mirrors osfmk/machine/pmap.h in XNU: each architecture provides its own
 * pmap implementation; this header selects the right one at compile time so
 * that vm/ code never has to name a specific architecture.
 */

#ifndef HAL_PMAP_H
#define HAL_PMAP_H

#if defined(__x86_64__)
#  include <hal/x86_64/pmap.h>
#else
#  error "pmap not implemented for this architecture"
#endif

#endif /* HAL_PMAP_H */
