join: build kernel 
	cat boot.bin kernel.bin > disk.img

build: boot.asm
	nasm boot.asm -o boot.bin

kernel: kernel.asm
	#gcc kernel.c -ffreestanding -fno-stack-protector -m32 -no-pie -O0 -nostdlib -nodefaultlibs -nostartfiles -o kernel.bin
	nasm kernel.asm -o kernel.bin