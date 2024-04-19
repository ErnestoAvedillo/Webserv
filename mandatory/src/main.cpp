/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/19 23:59:27 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

int main ()
{

	std::cout << "Hola, empezamos el proyecto" << std::endl;
	int listenfd, confd;
	pid_t child_pid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;


	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =	htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	while (true)
	{
		clilen = sizeof(cliaddr);
		confd = Accept(listenfd, (SA *)&cliaddr, &clilen);
		if ((child_pid = Fork()) == 0)
		{

			Close(listenfd);
			std::cout << "El valor de fd es: " << confd << std::endl;
			str_echo(confd);
			return (0);
		}
		Close(confd);
	}
}
