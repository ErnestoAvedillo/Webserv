/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/26 16:07:31 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"
#include <unistd.h>
void str_echo (int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE]= {0};
	std::fstream fileopen;
	std::string	line;
	std::string	line_2;
	std::string	complete = "";
	// std::stringstream ss;

	n = Readn(sockfd, buf, MAXLINE);
	std::cout << "Nueva impresion buffer" << std::endl;
	Request Instance;
	Instance.loadCompleteRequest(buf);
/*
	line = Instance.getPath();
	if (line == "/")
		line = "./www/index.html";
	else
		line = "./www" + line; 
	struct stat *fileinfo = NULL;
*/
	line = Instance.getFileName();
	int result = stat(line.c_str(), buf2);

	fileopen.open(line.c_str(), std::ios::in);

	std::string header;
	std::map<std::string, std::string> mimeTypes = create_filetypes();
	size_t j = line.find_last_of(".");
	//line.substr(n, line.length() - n);
	
	//std::cout << "Mime " << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << "<<>>"<< line.substr(j + 1, line.length() - j - 1) << std::endl;
	std::cout << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << std::endl;
	header = "HTTP/1.1 200 OK\nContent-Type: " + mimeTypes[line.substr(j + 1, line.length() - j - 1)] + "\n";// + "\nContent-Length: ";
	while (std::getline(fileopen,line_2,'\n'))
		complete += line_2;
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		;//header = header + "Content-Length: " + std::to_string(fileinfo->st_size) + "\n\n";
	else 
		header = header + "Content-Length: " + std::to_string(complete.length()) + "\n\n" + complete;
	
	size_t y = header.length();

	header += complete;
	 //+ complete;

	//std::stringstream sa;
	// sa << fileinfo->st_size;
	//header += sa.str() + "\n\n";
	//  complete;
	// std::cout << "----------------" << header << "-----------------" << std::endl;
	//line_2 = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12";
	
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		send(sockfd,header.c_str(), y + (long)fileinfo->st_size,0);
	else
		send(sockfd,header.c_str(),strlen(header.c_str()),0);
	// else
	// 	send(sockfd,header.c_str(), header.length(),1);
	//send(sockfd,"llo World\n",11,1);
	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
