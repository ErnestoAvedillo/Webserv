#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void)
{
	// const char *ip_str = "192.168.1.1";
	const char *servname = "127.0.0.1";
	// const char *servname = "localhost";
	const char *port = "8080";
    struct addrinfo hints, *res;
    struct sockaddr_in *addr;
    in_addr_t in_addr;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4

    int status = getaddrinfo(servname, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    addr = (struct sockaddr_in *)res->ai_addr;
    in_addr = addr->sin_addr.s_addr;

    printf("IP address in in_addr_t (network byte order): %u\n", in_addr);
    printf("IP address in in_addr_t (host byte order): %u\n", ntohl(in_addr));

    freeaddrinfo(res);
    return 0;
}