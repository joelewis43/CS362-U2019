CXX = gcc
CFLAGS= -Wall -fpic -coverage -lm -std=c99

main: testme.c
	$(CXX) $(CFLAGS) -o main testme.c
	./main	

clean:
	rm -rf main testme.g* testme.c.* *.out
	clear