CC=gcc -std=gnu11

mlfq: main.c program.c program.h
	$(CC) -o mlfq main.c