# variables para el input
N=4
TYPE=NL
SEED=0
DEBUG=0
ITERATIONS=10
LENLIST=$(shell expr $(N) / 2)
WEIGHT=5000

INPUT=$(TYPE)$(N)
VARIABLES=$(SEED) $(N) $(DEBUG) $(ITERATIONS) $(LENLIST) $(WEIGHT)
RUN=./main $(VARIABLES) < input/$(TYPE)/$(INPUT).txt > output/$(INPUT).$(SEED).solution

all: compile
	$(RUN)

compile:
	g++ src/main.cpp -o main -Wall

clean:
	rm -f main

# section of testing multiple instances
MOVIMIENTO=swapmatchesandrounds

testONE: compile
	for number in 4 6 10 12 14 16 ; do \
		for seed in $$(seq 0 9) ; do \
			echo instance: $$number seed: $$seed ; \
			./main $$seed $$number 1 100 $$(expr $$number / 2) < input/$(TYPE)/$(TYPE)$$number.txt > output/test.one/$(TYPE)$$number.$(MOVIMIENTO).$$seed.solution ; \
		done ; \
	done

testTWO: compile
	for number in 4 6 10 12 14 16 ; do \
		for seed in $$(seq 0 9) ; do \
			echo instance: $$number seed: $$seed ; \
			./main $$seed $$number 1 100 $$(expr $$number / 2) < input/$(TYPE)/$(TYPE)$$number.txt > output/test.two/$(TYPE)$$number.$(MOVIMIENTO).$$seed.solution ; \
		done ; \
	done

testTHREE: compile
	for number in 4 6 10 12 14 16 ; do \
		for seed in $$(seq 0 9) ; do \
			echo instance: $$number seed: $$seed ; \
			./main $$seed $$number 1 100 $$(expr $$number / 2) < input/$(TYPE)/$(TYPE)$$number.txt > output/test.final/$(TYPE)$$number.swapThree.$$seed.solution ; \
		done ; \
	done

resultados: compile
	for number in 4 6 10 12 14 16 ; do \
		for seed in $$(seq 0 9) ; do \
			echo instance: $$number seed: $$seed ; \
			./main $$seed $$number 1 50 $$(expr $$number / 2) < input/$(TYPE)/$(TYPE)$$number.txt > output/resultados/$(TYPE)$$number.$$seed.solution ; \
		done ; \
	done