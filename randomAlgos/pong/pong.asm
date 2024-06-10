section .data
    msg db "Hello world"
    lin db 0xa
    len equ $ -msg

section .text
global _start



_start:
    mov ebx, 1                  ; Set file descriptor to 1
    mov esi, 0                   ; Set ah to 0
    mov eax, 4

loop1:
    add esi, 1

    mov edx, esi
    mov ecx, msg
    mov eax, 4
    int 0x80

    mov edx, 1
    mov ecx, lin
    mov eax, 4
    int 0x80

    cmp esi, len
    jle loop1

    mov eax, 1                  ; Tell we're gonna exit
    int 0x80                    ; Ask it to fufil our request
