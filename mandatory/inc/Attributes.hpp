/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Attributes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 18:22:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/15 14:34:07 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
# include "utils.hpp"

class Attributes 
{
private:
	std::string name;
	struct stat attributes;
public:
	Attributes();
	Attributes(const std::string& filename);
	~Attributes();
	void setAttributesFromFile(const std::string& filename);
		
	std::string getName() ;
	bool getIsDir() ;
	std::string getSize() ;
	std::string getDevice() ;
	std::string getCreated() ;
	std::string getModified() ;
};