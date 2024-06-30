/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExtendedString.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:47:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 14:56:55 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "utils.hpp"

class ExtendedString : public std::string 
{
	public:
		ExtendedString();
		ExtendedString(const std::string &str);
		~ExtendedString();
		ExtendedString	&operator=(const std::string &str);
		
		bool						isNumber() const;
		bool						firstCharIsSign() const;
		void						replaceString(const std::string& , const std::string& );
		bool						validIPAddress();
		size_t						stringToSizeT();
		std::vector<ExtendedString>	splitString(char );
		void 						removeCharFromString(char);
		void 						removeBlanksAndTabs() ;
		int							count_chars(char c);
		void						replaceFirstString(const std::string& searchString, const std::string& replaceString);

};