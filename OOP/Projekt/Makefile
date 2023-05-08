COMPILER=g++
ARGS=
OFILES=main.o Order.o Employee.o Vegetarian.o Custom.o Dough.o Pizza.o Cheese.o Ingredient.o Service.o Rozvoz.o OnlineOrder.o Chef.o Vegetable.o Sauce.o Maso.o PersonalOrder.o PhoneOrder.o 
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
