FLAGS:= -lgmp -Wall

all: pa02.o test threads

threads: pa02-threads.c
	gcc $^ -o $@ $(FLAGS) -pthread

pa02.o: pa02.c
	gcc $^ -o $@ $(FLAGS)

test: grading-PA02-local.c
	gcc $^ -o $@ $(FLAGS)
	
threads-as-main: pa02-threads.c
	rm pa02.o
	gcc $^ -o pa02.o $(FLAGS) -pthread
	./test
	rm pa02.o

clean: 
	rm pa02.o test threads
