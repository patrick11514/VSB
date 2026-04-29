all:
	gcc main.c fat.c utils.c -o fat

populate-games: all ../apps/hello.bin ../apps/factory.bin
	$(MAKE) reset
	./fat ensure-dir GAMES
	./fat write-in-dir GAMES HELLO.BIN < ../apps/hello.bin
	./fat write-in-dir GAMES FACTORY.BIN < ../apps/factory.bin

clean:
	rm -f fat
	rm -f sd.img

reset:
	cp base.img sd.img