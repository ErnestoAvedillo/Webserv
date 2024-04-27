/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/25 00:17:45 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"
# include "../inc/Server.hpp"
# include "../inc/WebServer.hpp"

int main()
{
	// std::cout << "Hola, empezamos el proyecto" << std::endl;
	Request req;
	
	req.loadCompleteRequest("GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n\r\n");
	std::cout << req.getMethod() << std::endl;
	std::cout << req.getPath() << std::endl;
	std::cout << req.getProtocol() << std::endl;
	std::map<std::string, std::string> header = req.getHeader();
	for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	

	WebServer ws;
	Server server;
	server.setPort(2000);
	server.setHost("localhost");
	server.setServerName("localhost");
	server.setErrorPage("error.html");
	server.setClientBodySize(1000);
	server.setRoot("/var/www/html");
	server.setIndex("index.html");
	server.setIsDefault(true);
	server.printServer();
	// ws.servers.push_back(Server().);
	//ws.loadConfigFile("./conf/default");
	
	ws.servers.push_back(server);

	server.setPort(1000);
	server.setHost("localhost");
	server.setServerName("localhost");
	server.setErrorPage("error.html");
	server.setClientBodySize(1000);
	server.setRoot("/var/www/html");
	server.setIndex("index.html");
	server.setIsDefault(true);
	server.printServer();
	// ws.servers.push_back(Server().);
	//ws.loadConfigFile("./conf/default");
	
	server.setPort(8080);
	server.setHost("localhost");
	server.setServerName("localhost");
	server.setErrorPage("error.html");
	server.setClientBodySize(1000);
	server.setRoot("/var/www/html");
	server.setIndex("index.html");
	server.setIsDefault(true);
	server.printServer();
	
	ws.servers.push_back(server);
	ws.launchServers();
}

// int main()
// {
// 	std::cout << "Hola, empezamos el proyecto" << std::endl;
// 	int listenfd, confd;
// 	// pid_t child_pid;
// 	socklen_t clilen;
// 	struct sockaddr_in cliaddr, servaddr;
// 	int enable = 1;


// 	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
// 	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
//         std::cerr << "Error al configurar SO_REUSEADDR\n";
//         return 1;
// 	}
// 	bzero(&servaddr, sizeof(servaddr));
// 	servaddr.sin_family = AF_INET;
// 	servaddr.sin_addr.s_addr =	htonl(INADDR_ANY);
// 	servaddr.sin_port = htons(8080);
// 	if(bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
// 		std::cerr << "bind error" << std::endl;
// 	listen(listenfd, LISTENQ);
// 	while (true)
// 	{
// 		clilen = sizeof(cliaddr);
// 		confd = accept(listenfd, (SA *)&cliaddr, &clilen);
// 		// if ((child_pid = Fork()) == 0)
// 		// {
// 			str_echo(confd);
// 		// 	exit (0);
// 		// }
// 		Close(confd);
// 	}
// }
