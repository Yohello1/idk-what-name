##
# Linear Allocator moment
#
# @file
# @version 0.1
##


CC=gcc
CFLAGS=

LDER=-ld

OBJCOPIER=objcopy
OBJDUMPER=objdump


SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: alloc

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASMFLAGS) $< -c -o $@

alloc: $(OBJ)
	gcc $^ -o $@

run: prog.bin
	./alloc

clean:
	rm -f *.bin *.list *.o *.elf

# end
