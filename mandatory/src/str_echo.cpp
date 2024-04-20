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
	std::stringstream ss;

	fileopen.open(INDEX, std::ios::in);
	while (std::getline(fileopen,line,'\n'))
		complete += line;
	std::cout << complete << std::endl;
	n = Readn(sockfd, buf, MAXLINE);
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	ss << complete.length();
	header += ss.str() + "\n\n" + complete;
	line_2 = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHe\n";
	send(sockfd,header.c_str(),strlen(header.c_str()),1);
	
	send(sockfd,"llo World\n",11,1);
	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
