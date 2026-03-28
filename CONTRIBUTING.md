# Contributing to DANA

## Getting Started

```sh
make get-deps
make
make iso
make run
```

Requirements: `gcc`, `nasm`, `ld` (binutils), `xorriso`, `qemu-system-x86_64`, `limine`

## Code Style

- Language: C11, NASM for `hal/<arch>/boot_entry.S` only
- One clean comment line is sufficient for section grouping
- Indentation: 4 spaces, no tabs
- Maximum line length: 100 characters

Header guard format:
```c
#ifndef DIR_FILENAME_H
#define DIR_FILENAME_H
/* ... */
#endif /* DIR_FILENAME_H */
```

Copyright header for new files:
```c
/* path/to/file.c - Brief description
 * Copyright (C) <year> <Name> <email>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
```

## Architecture Rules

- `kern/` and `vm/` must not include anything from `hal/<arch>/` directly
- All platform-specific code lives in `hal/<arch>/`
- The only interface between the kernel core and a HAL is `hal/hal.h`
- Adding a new architecture means creating `hal/<arch>/` — no changes to `kern/` or `vm/`

## Submitting Changes

1. Fork the repository and create a branch from `main`
2. Keep commits focused — one logical change per commit
3. Ensure `make` produces zero warnings with the default flags
4. Ensure `make run` boots and prints the expected output in QEMU
5. Open a pull request with a clear description of what changed and why

## Reporting Issues

Open an issue with:
- A description of the problem
- Steps to reproduce
- Expected vs actual behavior
- Your host OS, GCC version, and QEMU version

## License

By contributing you agree that your changes will be licensed under the GNU General
Public License v3.0 or later. See [LICENSE](LICENSE).
