all:
	gcc main.c fat.c utils.c -o fat

clean:
	rm -f fat

reset:
	cp base.img sd.img

fuse: fuse_fat_bind.c fat.c utils.c fuse.c
	gcc -Wall fuse_fat_bind.c fat.c utils.c fuse.c  `pkg-config fuse3 --cflags --libs` -o fuse