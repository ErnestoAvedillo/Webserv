/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/24 23:33:58 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"
#include <unistd.h>

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

long FdGetFileSize(int fd)
{
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void str_echo (int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE]= {0};
	std::fstream fileopen;
	std::string	line;
	std::string	line_2;
	std::string	complete = "";
	std::streampos fileSize = 0;
	// std::stringstream ss;

	n = Readn(sockfd, buf, MAXLINE);
	std::cout << "Nueva impresion buffer" << std::endl;
	Request Instance;
	Instance.loadCompleteRequest(buf);

	line = Instance.getPath();
	if (line == "/")
		line = "./www/index.html";
	else
		line = "./www" + line; 
	struct stat *fileinfo = NULL;

	stat(line.c_str(), fileinfo);
	fileSize = GetFileSize(line);

	std::string header;
	std::map<std::string, std::string> mimeTypes = create_filetypes();
	size_t j = line.find_last_of(".");
	//line.substr(n, line.length() - n);
	
	//std::cout << "Mime " << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << "<<>>"<< line.substr(j + 1, line.length() - j - 1) << std::endl;
	std::cout << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << std::endl;
	header = "HTTP/1.1 200 OK\nContent-Type: " + mimeTypes[line.substr(j + 1, line.length() - j - 1)] + "\n";// + "\nContent-Length: ";
	
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
	{
		fileopen.open(line, std::ios::in | std::ios::binary);
		fileopen.seekg(0, std::ios::end);
		// fileSize = fileopen.tellg();
		// header = header + "Content-Length: " + std::to_string(fileinfo->st_size) + "\n\n";
	}
	else
	{
		fileopen.open(line, std::ios::in);
	}
	while (std::getline(fileopen,line_2,'\n'))
		complete += line_2;
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		 header = header + "Content-Length: " + "380437" + "\n\n";
	else 
		header = header + "Accept-Ranges: bytes\nContent-Length: " + std::to_string(complete.length()) + "\n\n" + complete;
	
	// size_t y = header.length();

	header += complete;
	 //+ complete;

	//std::stringstream sa;
	// sa << fileinfo->st_size;
	//header += sa.str() + "\n\n";
	//  complete;
	// std::cout << "----------------" << header << "-----------------" << std::endl;
	//line_2 = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12";
	
	
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		send(sockfd,header.c_str(), (long)fileinfo->st_size,0);
	else
		send(sockfd,header.c_str(),strlen(header.c_str()),0);
	// else
	// 	send(sockfd,header.c_str(), header.length(),1);
	//send(sockfd,"llo World\n",11,1);
	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
