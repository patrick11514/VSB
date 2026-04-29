CC ?= gcc
CFLAGS ?= -m32 -ffreestanding -fno-stack-protector -fno-pie -O0

all: hello.bin factory.bin

hello.bin: hello.c
	$(CC) $(CFLAGS) -c hello.c -o hello.o
	ld -m elf_i386 -T link.ld hello.o -o hello.elf
	objcopy -O binary hello.elf hello.bin

FACTORY_SRCS = factory.c factory_inventory.c factory_logic.c factory_io.c factory_render.c
FACTORY_OBJS = $(FACTORY_SRCS:.c=.o)

factory.bin: $(FACTORY_OBJS)
	ld -m elf_i386 -T link.ld $(FACTORY_OBJS) -o factory.elf
	objcopy -O binary factory.elf factory.bin

%.o: %.c factory.h
	$(CC) $(CFLAGS) -c $< -o $@
