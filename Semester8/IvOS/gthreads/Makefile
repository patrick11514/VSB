CFLAGS = -g -Wall

gthr_demo: gthr.o gthr_platform_linux.o gthr_switch.o main.o
	$(CC) -o $@ $^

.S.o:
	as -o $@ $^

.PHONY: clean
clean:
	rm -f *.o gthr_demo
