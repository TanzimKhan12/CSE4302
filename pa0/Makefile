HEADERS = simulator.h

default: simulator

simulator.o: simulator.c $(HEADERS)
	gcc -c simulator.c -o simulator.o -std=c99

simulator: simulator.o
	gcc simulator.o -o simulator -std=c99

clean:
	-rm -f simulator.o simulator
	-rm -f *out *txt
