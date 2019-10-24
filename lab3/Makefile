HEADERS = simulator.h

default: simulator

simulator.o: simulator.c $(HEADERS)
	gcc -c simulator.c -o simulator.o -ggdb -std=c99

simulator: simulator.o
	gcc simulator.o -o simulator -ggdb -std=c99

clean:
	-rm -f simulator.o
	-rm -f simulator
	-rm -f *.out mdump.txt cdump.txt pipe_trace.txt
