/* hal/thread.h - Architecture-independent thread HAL redirector
 * Copyright (C) 2026 AnmiTaliDev <anmitalidev@nuros.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef HAL_THREAD_H
#define HAL_THREAD_H

#if defined(__x86_64__)
#  include <hal/x86_64/thread.h>
#else
#  error "thread context not implemented for this architecture"
#endif

#endif /* HAL_THREAD_H */
