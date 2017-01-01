all: server client

CC = g++
CFLAGS = -std=c++11 -pthread -g

server:
	$(CC) -o $@ server.cpp $(CFLAGS)
client:
	$(CC) -o $@ client.cpp $(CFLAGS)

clean:
	rm -f server client
