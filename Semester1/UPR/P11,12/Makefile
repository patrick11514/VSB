COMPILER=gcc
ARGS=-Wall -Wextra -g -O2 -fsanitize=address -lSDL2 -lSDL2_ttf -lSDL2_image
OFILES=main.o
EXEC=main

$(EXEC): $(OFILES)
	$(COMPILER) $(ARGS) $(OFILES) -o $(EXEC)

%.o: %.c
	$(COMPILER) $(ARGS) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

clean: cleanOBJ cleanEXEC

all: clean $(EXEC)
