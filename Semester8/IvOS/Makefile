CC = gcc
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pie -O0

SRCS = kernel.c cli.c lib/string.c lib/convert.c drivers/vga.c drivers/serial.c drivers/keyboard.c drivers/ide.c drivers/fat_fs.c drivers/fat_io.c
OBJS = $(SRCS:.c=.o)

.PHONY: start join apps build link clean

start: join
	qemu-system-x86_64 -drive file=disk.img,format=raw -serial stdio

join: build link apps
	$(MAKE) -C fat populate-games
	cp fat/sd.img disk.img
	dd if=boot.bin of=disk.img bs=1 count=446 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

apps:
# HELLO PROGRAM
	$(CC) $(CFLAGS) -c apps/hello.c -o apps/hello.o
	ld -m elf_i386 -T apps/link.ld apps/hello.o -o apps/hello.elf
	objcopy -O binary apps/hello.elf apps/hello.bin
# FACTORIO
	$(CC) $(CFLAGS) -c apps/factory.c -o apps/factory.o
	ld -m elf_i386 -T apps/link.ld apps/factory.o -o apps/factory.elf
	objcopy -O binary apps/factory.elf apps/factory.bin


build: boot.asm
	nasm boot.asm -o boot.bin

switch.o: switch.asm
	nasm -f elf32 switch.asm -o switch.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

link: switch.o $(OBJS)
	ld -m elf_i386 -Ttext 0x7e00 --oformat binary switch.o $(OBJS) -o kernel.bin

clean:
	rm -f *.o drivers/*.o apps/*.o *.bin disk.img apps/*.elf apps/*.bin