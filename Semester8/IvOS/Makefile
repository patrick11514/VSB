CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pie -O0

SRCS = kernel.c drivers/vga.c drivers/serial.c drivers/keyboard.c drivers/ide.c
OBJS = $(SRCS:.c=.o)

start: join
	qemu-system-x86_64 -drive file=disk.img,format=raw -serial stdio

join: build link 
	cat boot.bin kernel.bin > disk.img

build: boot.asm
	nasm boot.asm -o boot.bin

switch.o: switch.asm
	nasm -f elf32 switch.asm -o switch.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

link: switch.o $(OBJS)
	ld -m elf_i386 -Ttext 0x7e00 --oformat binary switch.o $(OBJS) -o kernel.bin

clean:
	rm -f *.o drivers/*.o *.bin disk.img