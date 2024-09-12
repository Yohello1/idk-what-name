MEMORY {
  dram_space (rwx) : ORIGIN = 0x80000000, LENGTH = 256
}

SECTIONS {
  .text : {
    hello.o(.text.bios)
  } > dram_space
}
