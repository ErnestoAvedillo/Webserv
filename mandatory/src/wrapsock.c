/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapsock.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:04:33 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/16 19:02:45 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

int Socket (int family, int type, int protocol)
{
	int n;

	if (n = socket (family, type, protocol) < 0)
		err_sys("socket error");
	return n;
}

int Bind (int sockfd, const struct sockaddr *myaddr, socklen_t addrlen)
{
	int n;

	if (n = bind(sockfd, myaddr, addrlen) < 0)
		err_sys("bind error");
	return n;
}

int Listen(int sockfd, int backlog)
{
	int n;

	if (n = listen(sockfd, backlog) < 0)
		err_sys("listen error");
	return n;
}

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
	int n;

	if (n = accept(sockfd, cliaddr, addrlen) < 0)
		err_sys("accept error");
	return n;
}

pid_t Fork(void)
{
	pid_t n;

	if (n = fork() < 0)
		err_sys("fork error");
	return n;
}

int Execve(const char *pathname, char *const argv[], char *const envp[])
{
	int n;
 
	if (n = execve(pathname, argv, envp) < 0)
		err_sys("execve error");
	return n;
}

int Close(int sockfd)
{
	int n;

	if (n = clone (sockfd) < 0)
		err_sys("close socket error");
	return n;
}

ssize_t Readn(int filedes, void *buff, size_t nbytes)
{
	ssize_t n;

	if (n = readn(filedes, buff, nbytes))
		err_sys("readn error");
	return n;
}

ssize_t Writen(int filedes, void *buff, size_t nbytes)
{
	ssize_t n;
	
	if (n = writen(filedes, buff, nbytes))
		err_sys("writen error");
	return n;
}
ssize_t Readline(int filedes, void *buff, size_t maxlen)
{
	ssize_t n;

	if (n = readline(filedes, buff, maxlen))
		err_sys("readline error");
	return n;
}