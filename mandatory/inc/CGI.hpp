/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:47:27 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 15:00:16 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "../inc/utils.hpp"
#include <map>
#include "colors.h"
#include "StatusCodesDefinition.hpp"
#include <signal.h>
#include "ExtendedString.hpp"
#include "Environment.hpp"

class CGI : public Environment
{
	private:
		std::string							CGIFolder;
		std::string							fileName;
		std::string							fileArgs;
		bool								isCGI;
		std::vector <ExtendedString>		args;
		std::map<std::string, std::string>	CGIExtensions;

	public:
		CGI();
		CGI(const std::string &, const std::string &);
		CGI(const CGI &);
		~CGI();
		void							setCGIFolder(const std::string &);
		void							setFileName(const std::string&, const std::string&);
		void							setIsCGI(bool);
		bool							setIdentifyCGIFromFileName(const std::string &str); 
		bool							getIsCGI();
		void							setArgs(const std::string &);
		void							setCGIMapExtensions(std::string const &cgi_extension);
		std::string						getFileName();
		std::string						getCGIFolder();
		std::vector <ExtendedString>	getArgs();
		std::string						getCGIExtension(const std::string &);
		std::string						getFileExtension();
		std::string						execute();
		CGI 							*clone();
		static int						ChildPID;
		static void						alarm_handler(int);
		std::map<std::string, std::string>::iterator findCGIExtension(const std::string &);
};
