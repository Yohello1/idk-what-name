.global _start
.section .text.bios

_start:
    addi a0, x0, 0x68
    li a1, 0x10000000
    sb a0, (a1)                 ;

    addi a0, x0, 0x65
    sb a0, (a1)                 ;

    addi a0, x0, 0x6C
    sb a0, (a1)                 ;

    addi a0, x0, 0x6C
    sb a0, (a1)                 ;

    addi a0, x0, 0x6E
    sb a0, (a1)

    addi a0, x0, 0x10
    sb a0, (a1)

    li a2, 0x9
    li a3, 0x2

    addi a4, a2, 0
    sb a4, (a1)

loop:
    j loop
