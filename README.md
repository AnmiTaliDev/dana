# DANA

**Dana is Almost Not Apple** — a Mach microkernel targeting Darwin/XNU ABI compatibility.

## Architecture

DANA follows the Mach microkernel philosophy: the kernel provides tasks, threads,
IPC (ports and messages), and virtual memory. Higher-level services run in userspace.

## Building

```sh
make get-deps   # download include/limine.h
make            # build build/dana.elf
make iso        # build build/dana.iso
make run        # run in QEMU with serial output on stdout
```

**Requirements:** gcc, nasm, ld (binutils), xorriso, qemu-system-x86\_64, limine

## License

GNU General Public License v3.0 or later. See [LICENSE](LICENSE).
