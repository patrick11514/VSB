CC=gcc
FLAG=-Wall -Wextra -g -O2 -fsanitize=address
OBJFILES = main.o vector.o int_vector.o
HEADFILES = vector.h int_vector.h
NAME=vector

$(NAME): $(OBJFILES)
	$(CC) $(FLAG) -o $@ $(OBJFILES)

%.o: %.c $(HEADFILES)
	$(CC) $(FLAG) -c $<

cleanbin:
	rm -f $(NAME)

cleanobj:
	rm -f *.o

clean: cleanbin cleanobj

all: clean $(NAME)
