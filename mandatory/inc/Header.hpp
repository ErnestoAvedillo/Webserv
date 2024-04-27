/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:59 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 18:22:16 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SendFile.hpp"
#include "commonFunctions.hpp"
#include "itos.hpp"
#include <iostream>
#include <fstream>
#include <vector>

class Header
{
	private:
		std::string Type;
		FileName	FileName;
		std::string HTTPvers;
		std::string MimmeType;
		size_t		ContentLength;
	public:
		Header();
		Header(Header const &);
		// Header &operator=(Header const &);
		~Header();
		std::string getType();
		std::string getFileName() const ;
		void setFilename(std::string filename);
		std::string getHTTPvers();
		int getContentLength();
		void setType(std::string);
		void setHTTPvers(std::string);
		void setContentLength(int);
		void setHeader(std::string);
		void setHeader(Header const &);
		std::string getHeader();

};

std::vector<std::string> splitString(const std::string &str, char delimiter);
