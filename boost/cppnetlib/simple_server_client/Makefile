all: server.bin client.bin

CC = c++
LIBS = -lcppnetlib-uri -lboost_system -lboost_thread
FLAGS = -std=c++11 -pthread -g

server.bin:
	$(CC) -o $@ server.cpp -lcppnetlib-server-parsers $(LIBS) $(FLAGS)

client.bin:
	$(CC) -o $@ client.cpp -lcppnetlib-client-connections $(LIBS) $(FLAGS)

clean:
	rm -rf *.bin *.bin.*

