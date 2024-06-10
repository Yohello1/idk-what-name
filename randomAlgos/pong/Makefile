##
# Pong
#
# @file
# @version 0.1

all: pong1


pong.o: pong.asm
	nasm -f elf64 pong.asm -o pong.o

pong1: pong.o
	ld -o pong pong.o

# end
