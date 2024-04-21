/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/21 14:11:00 by eavedill         ###   ########.fr       */
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

	line = Instance.getPath();
	if (line == "/")
		line = "./www/index.html";
	else
		line = "./www" + line; 
	struct stat *fileinfo = NULL;

	stat(line.c_str(), fileinfo);
	fileopen.open(line, std::ios::in);

	std::string header;
	std::map<std::string, std::string> mimeTypes = create_filetypes();
	size_t j = line.find_last_of(".");
	//line.substr(n, line.length() - n);
	
	std::cout << "Mime " << mimeTypes[line.substr(j + 1, line.length() - j - 1)] << "<<>>"<< line.substr(j + 1, line.length() - j - 1) << std::endl;
	header = "HTTP/1.1 200 OK\nContent-Type: " + mimeTypes[line.substr(j + 1, line.length() - j - 1)];// + "\nContent-Length: ";
	while (std::getline(fileopen,line_2,'\n'))
		complete += line_2;
	if (mimeTypes[line.substr(j + 1, line.length() - j - 1)] == "text/html")
		header = header + "\nContent-Length: " + itos(complete.length());
	

	std::stringstream sa;
	// sa << fileinfo->st_size;
	header += sa.str() + "\n\n";
	//  complete;
	// std::cout << "----------------" << header << "-----------------" << std::endl;
	//line_2 = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12";
	send(sockfd,header.c_str(),strlen(header.c_str()),1);
	if (mimeTypes[line.substr(j, line.length() - j)] == "image/png")
		send(sockfd,complete.c_str(), (long)fileinfo->st_size,1);
	else
		send(sockfd,complete.c_str(), complete.length(),1);
	//send(sockfd,"llo World\n",11,1);
	if (n < 0)
		std::cerr << "str_Echo:read error\n";
}
