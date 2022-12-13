COMPILER=gcc
ARGS=-Wall -Wextra -g -O2 -fsanitize=address
OFILES=main.o vector.o
HFILES=vector.h
EXEC=main

$(EXEC): $(OFILES)
	$(COMPILER) $(HFILES) $(ARGS) $(OFILES) -o $(EXEC)

%.o: %.c
	$(COMPILER) $(ARGS) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

clean: cleanOBJ cleanEXEC

all: clean $(EXEC)
