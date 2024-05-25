/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 07:31:33 by marvin            #+#    #+#             */
/*   Updated: 2024/04/21 12:26:13 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main ()
{
	try
	{
		ImportData filetype (',');
		filetype.readFile("text.csv");
		filetype.readFile("image.csv");
		filetype.readFile("font.csv");
		filetype.readFile("application.csv");
		filetype.readFile("audio.csv");
		filetype.readFile("message.csv");
		filetype.readFile("model.csv");
		filetype.readFile("multipart.csv");
		filetype.readFile("video.csv");
		std::map<std::string, std::string>::iterator itb = filetype.getBegin();
		std::map<std::string, std::string>::iterator ite = filetype.getEnd();
		std::cout << "#include <map>" << std::endl; 
		std::cout << std::endl; 
		std::cout << "std::map<std::string, std::string> filetypes " << std::endl; 
		while (itb != ite)
		{
			std::cout << "filetypes[\"" << itb->first << "\"] = \"" << itb->second << "\";" << std::endl;
			itb++;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
