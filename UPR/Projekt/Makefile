COMPILER=gcc
ARGS=-Wall -Wextra -g -O2 -fsanitize=address -lSDL2
OFILES=main.o dynamicarray.o breakout.o files.o global.o highscores.o
HFILES=dynamicarray.h breakout.h files.h global.h highscores.h
EXEC=main

$(EXEC): $(OFILES)
	$(COMPILER) $(ARGS) $(OFILES) -o $(EXEC)

%.o: %.c $(HFILES)
	$(COMPILER) $(ARGS) $(HFILES) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

clean: cleanOBJ cleanEXEC

all: clean $(EXEC)
