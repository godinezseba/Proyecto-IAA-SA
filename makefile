# variables para el input
N=4
TYPE=NL
SEED=0
DEBUG=0
ITERATIONS=10

INPUT=$(TYPE)$(N)
RUN=./main $(SEED) $(N) $(DEBUG) $(ITERATIONS) < input/$(TYPE)/$(INPUT).txt > output/$(INPUT).solution

all: compile
	$(RUN)

test:
	g++ src/test.cpp -o test -Wall
	./test

compile:
	g++ src/main.cpp -o main -Wall

clean:
	rm -f main