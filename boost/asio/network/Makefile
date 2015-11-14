all: server client

server:
	c++ -o server daytime_async_server.cpp -lboost_system -lboost_thread -g
client:
	c++ -o client daytime_client.cpp -lboost_system -g
clean:
	rm -f server client
