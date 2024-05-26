/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:25 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/25 23:15:31 by eavedill         ###   ########.fr       */
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
	int fd;
	std::string file_name;
	std::vector <std::string> args;
	std::vector <std::string> env;
public:
	CGI(const std::string& str);
	~CGI();
	std::string execute();
};
