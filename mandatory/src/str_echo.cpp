/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 18:39:07 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/unp.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "../inc/Request.hpp"


void str_echo (int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE]= {0};
	std::fstream fileopen;
	std::string	line;
	std::string	line_2;
	std::string	complete = "";
	// std::stringstream ss;
	n = read(sockfd, buf, MAXLINE);
	if (n < 0)
		std::cerr << "read error\n";
	std::cout << "Nueva impresion buffer" << buf << std::endl;
	Request Instance;
	Instance.loadCompleteRequest(buf);
	Instance.getHeader();

	std::string header;
	std::map<std::string, std::string> mimeTypes = create_filetypes();
	size_t j = line.find_last_of(".");
	//line.substr(n, line.length() - n);
	
	std::cout << "Mime " << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << "<<>>"<< line.substr(j + 1, line.length() - j - 1) << std::endl;
	
	std::cout << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << std::endl;
	header = "HTTP/1.1 200 OK\nContent-Type: " + mimeTypes[line.substr(j + 1, line.length() - j - 1)] + "\n";// + "\nContent-Length: ";
	while (std::getline(fileopen,line_2,'\n'))
		complete += line_2;
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		;//header = header + "Content-Length: " + std::to_string(fileinfo->st_size) + "\n\n";
	else 
		header = header + "Content-Length: " + std::to_string(complete.length()) + "\n\n" + complete;
	
	//size_t y = header.length();

	header += complete;

	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
