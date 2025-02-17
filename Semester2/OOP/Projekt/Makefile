COMPILER=g++
ARGS=-Wall -Wextra -g -O2 -fsanitize=address
OFILES=main.o Vegetarian.o Custom.o Order.o Service.o Vegetables.o Pizza.o PersonalOrder.o Meat.o Employee.o Cheese.o Sauce.o Pizzeria.o Chef.o Dough.o OnlineOrder.o Ingredient.o AbstractEmployee.o AbstractOrder.o
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
