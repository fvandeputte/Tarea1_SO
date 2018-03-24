CC=gcc -std=gnu11

OPT=-g

CFLAGS=-Wunused -Wall $(OPT)

mlfq: main.c
	$(CC) $(CFLAGS) mlfq main.c