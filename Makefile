CXX = g++
CXXFLAGS = -Wall -Wextra -pthread

all: server client

server: server_main.cpp server.hpp
	$(CXX) $(CXXFLAGS) server_main.cpp -o server

client: client_main.cpp client.hpp
	$(CXX) $(CXXFLAGS) client_main.cpp -o client

clean:
	rm -f server client

.PHONY: all clean
