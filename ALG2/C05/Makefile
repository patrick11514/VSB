COMPILER=g++
ARGS=
OFILES=main.o ComparsionCountingSort.o DistributionCountingSort.o
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
