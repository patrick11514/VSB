#build
build: global.o files.o highscores.o main.o
	gcc global.o files.o highscores.o main.o -o main -lSDL2 -Wall -Wextra -g -O2 -fsanitize=address

#modules
global.o: 
	gcc -c global.c -o global.o
files.o:
	gcc -c files.c -o files.o
highscores.o:
	gcc -c highscores.c -o highscores.o
main.o:
	gcc -c main.c -o main.o

#clean
clean:
	rm *.o