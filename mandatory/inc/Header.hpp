/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:59 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/13 18:59:53 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SendFile.hpp"
#include "commonFunctions.hpp"
#include "toString.hpp"
#include <iostream>
#include <fstream>
#include <vector>

class Header
{
	private:
		std::string Type;
		FileName	filename;
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
