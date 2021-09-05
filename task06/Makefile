CFLAGS = -g -Wall
all: table philosopher
table: table.c
	gcc $(CFLAGS) table.c -o table
philosopher: philosopher.c
	gcc $(CFLAGS) philosopher.c -o philosopher
clean:
	rm -f table philosopher
phil: philosopher
	./philosopher 0 & ./philosopher 1 & ./philosopher 2 & ./philosopher 3 & ./philosopher 4 &