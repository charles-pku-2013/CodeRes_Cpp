all: server client

CC = c++
LIBS = -lrt
FLAGS = -std=c++11 -pthread -O3 -g

server:
	$(CC) -o $@.bin server.cpp $(LIBS) $(FLAGS)

client:
	$(CC) -o $@.bin client.cpp $(LIBS) $(FLAGS)

clean:
	rm -f *.bin
