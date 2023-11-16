COMPILER=g++
ARGS=-fsanitize=address -g -Wall -Wextra
OFILES=main.o HeightMap.o Cube.o
EXEC=main

$(EXEC): $(OFILES)
	$(COMPILER) $(ARGS) $(OFILES) -o $(EXEC)

%.o: %.cpp
	$(COMPILER) $(ARGS) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

clean: cleanOBJ cleanEXEC

all: clean $(EXEC)
