    global    _start

    section .data
hello:
    db "Hello, World", 10

    section   .text
_start:
    mov       rax, 1
    mov       rdi, 1
    mov       rsi, hello
    mov       rdx, 13
    syscall
    mov       rax, 60
    xor       rdi, rdi
    syscall
