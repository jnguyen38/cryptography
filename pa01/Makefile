FLAGS:= -lgmp -Wall

all: pa01.o test run

pa01.o: pa01.c
	gcc $^ -o $@ $(FLAGS)

test: gradingPA01-local.c
	gcc $^ -o $@ $(FLAGS)

run: test_run.c
	gcc $^ -o $@ $(FLAGS)

clean: 
	rm pa01.o test
