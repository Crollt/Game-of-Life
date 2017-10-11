CC=gcc --std=c99 -g

life.o: life.c
	$(CC) life.c -w -o life -lncurses

warn: life.c
	$(CC) life.c -o life -lncurses

clean:
	rm -rf life
