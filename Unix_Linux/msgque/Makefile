all: sender receiver

CC=gcc

sender:
	$(CC) -o $@ msgsend.c -g
receiver:
	$(CC) -o $@ msgreceive.c -g

clean:
	rm -f sender receiver

