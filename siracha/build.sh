#!/usr/bin/env sh


# Note this is only known to work on my dumpster fire of a computer

# DTS dump
qemu-system-riscv64 -machine virt,dumpdtb=src/virt.dtb
dtc src/virt.dtb > src/virt.dts

# Compiling!
ldc2 -Oz -betterC -mtriple=riscv64-unknown-elf -mattr=+m,+a,+c --code-model=medium -c src/dstart.d
riscv64-unknown-elf-as -mno-relax -march=rv64imac src/start.s -c -o src/start.o
riscv64-unknown-elf-ld -Tsrc/link.ld start/start.o start/dstart.o -o prog.elf
