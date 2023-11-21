COMPILER=g++
ARGS=-O3
OFILES=main.o HeightMap.o Cube.o types.o
EXEC=main

$(EXEC): $(OFILES)
	$(COMPILER) $(ARGS) $(OFILES) -o $(EXEC)

%.o: %.cpp
	$(COMPILER) $(ARGS) -c $<

cleanOBJ:
	rm -f $(OFILES)

cleanEXEC:
	rm -f $(EXEC)

cleanDoxygen:
	rm -rf html latex

clean: cleanOBJ cleanEXEC cleanDoxygen

all: clean $(EXEC)

doxygen: Doxyfile
	doxygen Doxyfile

pdf: doxygen
	cd latex && make pdf
	mv latex/refman.pdf ./Documentation.pdf

