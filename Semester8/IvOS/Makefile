CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pie -O0

SRCS = kernel.c cli.c lib/string.c lib/convert.c drivers/vga.c drivers/serial.c drivers/keyboard.c drivers/ide.c drivers/fat_fs.c drivers/fat_io.c
OBJS = $(SRCS:.c=.o)

.PHONY: start join apps build link clean

start: 
	qemu-system-x86_64 -drive file=disk.img,format=raw -serial stdio

all: join 

join: build link apps
	$(MAKE) -C fat populate-games
	cp fat/sd.img disk.img
	dd if=boot.bin of=disk.img bs=1 count=446 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

apps:
	$(MAKE) -C apps


build: boot.asm
	nasm boot.asm -o boot.bin

switch.o: switch.asm
	nasm -f elf32 switch.asm -o switch.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

link: switch.o $(OBJS)
	ld -m elf_i386 -Ttext 0x7e00 --oformat binary switch.o $(OBJS) -o kernel.bin

clean:
	rm -f *.o drivers/*.o apps/*.o *.bin disk.img apps/*.elf apps/*.bin lib/*.o
	cd fat && make clean