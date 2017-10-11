CC=gcc --std=c99 -g

life:
	$(CC) life.c -o life -lncurses

clean:
	rm -rf test
