	#include "../inc/ListeningSocket.hpp"
	
	ListeningSocket::ListeningSocket(int port) : port(port), socketFd(-1), kq(-1) {}

	bool ListeningSocket::startListening() {
		// Create a socket
		socketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (socketFd == -1) {
			std::cerr << "Failed to create socket" << std::endl;
			return false;
		}

		// Set up the server address
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(port);

		// Bind the socket to the server address
		if (bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
			std::cerr << "Failed to bind socket to address" << std::endl;
			return false;
		}

		// Start listening for incoming connections
		if (listen(socketFd, 5) < 0) {
			std::cerr << "Failed to start listening" << std::endl;
			return false;
		}

		// Create a new kqueue
		kq = kqueue();
		if (kq == -1) {
			std::cerr << "Failed to create kqueue" << std::endl;
			return false;
		}

		// Add the socket file descriptor to the kqueue
		struct kevent event;
		EV_SET(&event, socketFd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
		if (kevent(kq, &event, 1, nullptr, 0, nullptr) == -1) {
			std::cerr << "Failed to add socket to kqueue" << std::endl;
			return false;
		}

		std::cout << "Listening on port " << port << std::endl;
		return true;
	}

	void ListeningSocket::stopListening() {
		if (socketFd != -1) {
			close(socketFd);
			socketFd = -1;
		}
		if (kq != -1) {
			close(kq);
			kq = -1;
		}
	}

	void ListeningSocket::handleEvents() {
		struct kevent events[10];
		while (true) {
			int numEvents = kevent(kq, nullptr, 0, events, 10, nullptr);
			if (numEvents == -1) {
				std::cerr << "Failed to wait for events" << std::endl;
				break;
			}

			for (int i = 0; i < numEvents; ++i) {
				if (events[i].ident == (uintptr_t) socketFd) {
					// Accept incoming connection
					sockaddr_in clientAddress;
					socklen_t clientAddressLength = sizeof(clientAddress);
					int clientSocketFd = accept(socketFd, (struct sockaddr*)&clientAddress, &clientAddressLength);
					if (clientSocketFd == -1) {
						std::cerr << "Failed to accept connection" << std::endl;
						continue;
					}

					// Handle the connection
					handleConnection(clientSocketFd);
				}
			}
		}
	}
