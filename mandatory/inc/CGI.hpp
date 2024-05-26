/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:25 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/26 12:46:52 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../inc/utils.hpp"

class CGI {
private:
	std::string file_name;
	std::vector <std::string> args;
public:
	CGI();
	CGI(const std::string& str);
	~CGI();
	void setFileName(const std::string& str);
	std::string getFileName();
	void setArgs(const std::vector <std::string>&);
	std::vector <std::string> getArgs();
	std::string execute();
	
};
