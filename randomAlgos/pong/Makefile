##
# Pong
#
# @file
# @version 0.1

all: pong1


pong.o: pong.asm
	nasm -f elf32 -o pong.o pong.asm

pong1: pong.o
	ld -m elf_i386 -o pong pong.o

# end
