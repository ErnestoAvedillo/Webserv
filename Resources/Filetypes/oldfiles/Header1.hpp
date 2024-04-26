/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:10:59 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/23 22:55:29 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include  "../inc/main.hpp"

class Header
{
	private:
		std::map<std::string, std::string> Request;

	public:
		Header();
		Header(Header const &);
		Header &operator=(Header const &);
		~Header();
		void Header::loadKey(std::string &str1, std::string &str2);
		std::string getKey(std::map<std::string, std::string>::iterator);
		std::map<std::string, std::string>::iterator getBegin()  ;
		std::map<std::string, std::string>::iterator getEnd() ;
		std::string getValue(std::string const &);
};
