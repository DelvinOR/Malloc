CC = gcc
CFLAGS = -g

all: memgrind

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) mymalloc.c -c

memgrind: memgrind.c mymalloc.o
	$(CC) $(CFLAGS) mymalloc.o memgrind.c -o memgrind

clean:
	rm -f mymalloc.o memgrind
