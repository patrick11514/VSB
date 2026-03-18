start: join
	qemu-system-x86_64 -drive file=disk.img,format=raw

join: build link 
	cat boot.bin kernel.bin > disk.img

build: boot.asm
	nasm boot.asm -o boot.bin

switch: switch.asm
	nasm -f elf32 switch.asm -o switch.o

kernel: kernel.c
	gcc -m32 -ffreestanding -fno-stack-protector -fno-pie -O0 -c kernel.c -o kernel.o

link: switch kernel
	ld -m elf_i386 -Ttext 0x7e00 --oformat binary switch.o kernel.o -o kernel.bin