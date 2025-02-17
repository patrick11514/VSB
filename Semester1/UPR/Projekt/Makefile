COMPILER=gcc
ARGS=-Wall -Wextra -g -O2 -fsanitize=address -lSDL2 -lSDL2_ttf -lSDL2_image
OFILES=dynamicarray.o breakout.o files.o global.o highscores.o assets.o vars.o args.o main.o
HFILES=files.h highscores.h dynamicarray.h breakout.h global.h assets.h vars.h args.h
EXEC=main

$(EXEC): $(OFILES) $(HFILES)
	$(COMPILER) $(ARGS) $(OFILES) $(HFILES) -o $(EXEC)

%.o: %.c
	$(COMPILER) $(ARGS) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

clean: cleanOBJ cleanEXEC

all: clean $(EXEC)
