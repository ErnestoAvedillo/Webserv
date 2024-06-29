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
		std::string CGIFolder;
		std::string fileName;
		std::string fileArgs;
		bool isCGI;
		std::vector <ExtendedString> args;
		std::map<std::string, std::string> CGIExtensions;

	public:
		CGI();
		CGI(const std::string &, const std::string &);
		CGI(const CGI &);
		~CGI();
		//------ Setters and Getters ------//
		//setters
		void setCGIFolder(const std::string &);
		void setFileName(const std::string&, const std::string&);
		void setIsCGI(bool);
		bool setIdentifyCGIFromFileName(const std::string &str); 
		bool getIsCGI();
		//bool setIsCGI(const std::string &str);
		void setArgs(const std::string &);
		void setCGIMapExtensions(std::string const &cgi_extension);
		//getters
		std::string getFileName();
		std::string getCGIFolder();
		std::vector <ExtendedString> getArgs();
		std::string getCGIExtension(const std::string &);
		std::string getFileExtension();
		std::map<std::string, std::string>::iterator findCGIExtension(const std::string &);
		//------ OTHER Methods ------//
		std::string execute();
		CGI *clone();

		static int ChildPID;
		static void alarm_handler(int);
};
