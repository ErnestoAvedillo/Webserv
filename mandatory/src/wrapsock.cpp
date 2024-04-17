/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapsock.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:04:33 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/18 00:03:24 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

int Socket (int family, int type, int protocol)
{
	int n;

	if ((n = socket(family, type, protocol)) < 0)
		std::cerr << "socket error";
	return n;
}

int Bind (int sockfd, const struct sockaddr *myaddr, socklen_t addrlen)
{
	int n;

	if ((n = bind(sockfd, myaddr, addrlen)) < 0)
		std::cerr << "bind error\n";
	return n;
}

int Listen(int sockfd, int backlog)
{
	int n;

	if ((n = listen(sockfd, backlog)) < 0)
		std::cerr << "listen error\n";
	return n;
}

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
	int n;

	if ((n = accept(sockfd, cliaddr, addrlen)) < 0)
		std::cerr << "accept error\n";
	return n;
}

pid_t Fork(void)
{
	pid_t n;

	if ((n = fork()) < 0)
		std::cerr << "fork error\n";
	return n;
}

int Execve(const char *pathname, char *const argv[], char *const envp[])
{
	int n;
 
	if ((n = execve(pathname, argv, envp)) < 0)
		std::cerr << "execve error\n";
	return n;
}

int Close(int sockfd)
{
	int n;

	if ((n = close (sockfd)) < 0)
		std::cerr << "close socket error\n";
	return n;
}

ssize_t Readn(int filedes, void *buff, size_t nbytes)
{
	ssize_t n;

	if ((n = readn(filedes, buff, nbytes)) < 0)
		std::cerr << "readn error\n";
	return n;
}

ssize_t Writen(int filedes, void *buff, size_t nbytes)
{
	ssize_t n;
	
	if ((n = writen(filedes, buff, nbytes)) < 0)
		std::cerr << "writen error\n";
	return n;
}
ssize_t Readline(int filedes, void *buff, size_t maxlen)
{
	ssize_t n;

	if ((n = readline(filedes, buff, maxlen)) < 0)
		std::cerr << "readline error\n";
	return n;
}
