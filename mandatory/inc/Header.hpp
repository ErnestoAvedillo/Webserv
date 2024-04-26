/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:59 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/26 15:49:15 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include  "../inc/main.hpp"

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
		Header &operator=(Header const &);
		Header(std::string, std::string, std::string);
		~Header();
		std::string getType();
		std::string getFileName();
		std::string getHTTPvers();
		int getContentLength();
		void setType(std::string);
		void setFileName(std::string);
		void setHTTPvers(std::string);
		void setContentLength(int);
		void setHeader(std::string);
		void setHeader(Header const &);
		std::string getHeader();

};
