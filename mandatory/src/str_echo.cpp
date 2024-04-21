/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/19 23:54:17 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

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
	std::map<std::string, std::string> Header;

	Header = Instance.getHeader();
	std::cout << "#######################################################" << std::endl;
	for (std::map<std::string, std::string>::iterator it = Header.begin(); it != Header.end(); ++it)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
	std::cout << "#######################################################" << std::endl;
	// std::cout << buf << std::endl;
	std::stringstream ss(buf);
	// std::vector<std::string> split_string;
	
	std::getline(ss,line, ' ');
	std::getline(ss,line, ' ');

	// line = "";
	if (line == "/")
		line = "./www/index.html";
	else
		line = "./www" + line; 
	std::cout << "----------------______________-----------------" << std::endl;
	std::cout << "----------------" << line << "-----------------" << std::endl;
	
	fileopen.open(line, std::ios::in);
	line = "." + line;
	std::string header;
	if (line.find(".png") != std::string::npos)
		header = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: ";
	else
		header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	
	while (std::getline(fileopen,line_2,'\n'))
		complete += line_2;
	std::stringstream sa;
	sa << complete.length();
	header += sa.str() + "\n\n" + complete;
	// std::cout << "----------------" << header << "-----------------" << std::endl;
	//line_2 = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12";
	send(sockfd,header.c_str(),strlen(header.c_str()),1);
	
	//send(sockfd,"llo World\n",11,1);
	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
