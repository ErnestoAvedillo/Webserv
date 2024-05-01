#include "../inc/ListeningSocket.hpp"

#ifdef LNX
ListeningSocket::ListeningSocket(int port) : port(port), socketFd(-1), epollFd(-1) 
{
	std::cout << "ListeningSocket constructor for port " << port << " created." << std::endl;
}



bool ListeningSocket::startListening()
{
	// Create a socket
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return false;
	}

	// Set up the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	// Bind the socket to the server address
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cerr << "Failed to bind socket to address" << std::endl;
		return false;
	}

	// Start listening for incoming connections
	if (listen(socketFd, MAX_CONNECTIONS) < 0)
	{
		std::cerr << "Failed to start listening" << std::endl;
		return false;
	}

	// Create a new epoll instance
	epollFd = epoll_create1(0);
	if (epollFd == -1)
	{
		std::cerr << "Failed to create epoll instance" << std::endl;
		return false;
	}

	// Add the socket file descriptor to the epoll instance
	epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = socketFd;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event) == -1)
	{
		std::cerr << "Failed to add socket to epoll" << std::endl;
		return false;
	}

	std::cout << "Listening on port " << port << std::endl;
	return true;
}
void ListeningSocket::stopListening()
{
	if (socketFd != -1)
	{
		close(socketFd);
		socketFd = -1;
	}
	if (epollFd != -1)
	{
		close(epollFd);
		epollFd = -1;
	}
}
void ListeningSocket::handleEvents()
{
	epoll_event events[MAX_EVENTS];
	while (true)
	{
		int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if (numEvents == -1)
		{
			std::cerr << "Failed to wait for events" << std::endl;
			break;
		}

		for (int i = 0; i < numEvents; ++i)
		{
			if (events[i].data.fd == socketFd)
			{
				// Accept incoming connection
				sockaddr_in clientAddress;
				socklen_t clientAddressLength = sizeof(clientAddress);
				int clientSocketFd = accept(socketFd, (struct sockaddr *)&clientAddress, &clientAddressLength);
				if (clientSocketFd == -1)
				{
					std::cerr << "Failed to accept connection" << std::endl;
					continue;
				}

				// Add the client socket file descriptor to the epoll instance
				epoll_event clientEvent;
				clientEvent.events = EPOLLIN;
				clientEvent.data.fd = clientSocketFd;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocketFd, &clientEvent) == -1)
				{
					std::cerr << "Failed to add client socket to epoll" << std::endl;
					close(clientSocketFd);
					continue;
				}
			}
			else
			{
				// Handle client connection
				handleConnection(events[i].data.fd);
			}
		}
	}
}

#else
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
		kevent_t event;
		EV_SET(&event, socketFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
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
		kevent_t events[10];
		while (true) {
			int numEvents = kevent(kq, NULL, 0, events, 10, NULL);
			if (numEvents == -1) {
				std::cerr << "Failed to wait for events" << std::endl;
				break;
			}

			for (int i = 0; i < numEvents; ++i) {
				if (events[i].ident == (unsigned int) socketFd) {
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
#endif

void ListeningSocket::handleConnection(int clientSocketFd)
{
	// Handle the connection here
	// For example, read/write data from/to the client
	n = read(clientSocketFd, buffer, sizeof(buffer));
	if (n == -1)
	{
		std::cerr << "Failed to read from client" << std::endl;
		return;
	}
	else
	{
		std::cout << "Received " << n << "\n bytes: " << buffer << std::endl;
		std::string buffer = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
		send(clientSocketFd, buffer.c_str(), buffer.size(), 0);
	}
	// Remember to close the client socket when done
	if (close(clientSocketFd) == -1)
	{
		std::cerr << "Failed to close client socket" << std::endl;
	}
	else
	{
		std::cout << "Closed client socket" << std::endl;
	}
}