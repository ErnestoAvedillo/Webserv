#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <unistd.h>
#include <map>

class ListeningSocket {
public:
ListeningSocket(int port);
bool startListening();
void stopListening();
void handleEvents();

private:
	int port;
	int socketFd;
	int kq;
	char buffer[1024];
	int n;

	void handleConnection(int clientSocketFd) {
		// Handle the connection here
		// For example, read/write data from/to the client
		n = read(clientSocketFd, buffer, sizeof(buffer));
		if (n == -1) {
			std::cerr << "Failed to read from client" << std::endl;
			return;
		}
		else
		{
				std::cout << "Received " << n << "\n bytes: " << buffer << std::endl;
				std::string buffer = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>\n";
				send(clientSocketFd, buffer.c_str(), buffer.size(), 0);
		}
		// Remember to close the client socket when done
		if(close(clientSocketFd) == -1) {
			std::cerr << "Failed to close client socket" << std::endl;
		}
		else {
			std::cout << "Closed client socket" << std::endl;
		}
	}
};
/*
int main() {
		std::map<int, ListeningSocket> socketMap;
		socketMap[8080] = ListeningSocket(8080);
		socketMap[8081] = ListeningSocket(8081);
		socketMap[8082] = ListeningSocket(8082);

		for (auto& pair : socketMap) {
			if (pair.second.startListening()) {
				pair.second.handleEvents();
			}
			pair.second.stopListening();
		}

		return 0;
	}
}*/
