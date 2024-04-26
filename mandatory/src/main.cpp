/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/24 19:35:09 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

int main()
{

	std::cout << "Hola, empezamos el proyecto" << std::endl;
	int listenfd, confd;
	// pid_t child_pid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	int enable = 1;


	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        std::cerr << "Error al configurar SO_REUSEADDR\n";
        return 1;
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr =	htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);
	if(bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
		std::cerr << "bind error" << std::endl;
	listen(listenfd, LISTENQ);
	while (true)
	{
		clilen = sizeof(cliaddr);
		confd = accept(listenfd, (SA *)&cliaddr, &clilen);
		str_echo(confd);
		Close(confd);
	}
}
