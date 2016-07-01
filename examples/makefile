CC = gcc
CCFLAGS = -g -Wall
EXE = example1 example2 example3

all : $(EXE)

example1: hashtable.o
	$(CC) $(CCFLAGS) -o example1 example1.c hashtable.o

example2: hashtable.o
	$(CC) $(CCFLAGS) -o example2 example2.c hashtable.o

example3: hashtable.o
	$(CC) $(CCFLAGS) -o example3 example3.c hashtable.o

hashtable.o:
	$(CC) $(CCFLAGS) -c -o hashtable.o ../hashtable.c

clean:
	rm -f  *.o $(EXE)
