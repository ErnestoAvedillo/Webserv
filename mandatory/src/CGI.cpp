/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:49:55 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 15:28:58 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

CGI::CGI() : Environment()
{
	this->fileName = "";
	this->fileArgs = "";
	this->isCGI = false;
	this->CGIFolder = "/cgi/";
	this->setCGIMapExtensions("cgi");
}

CGI::CGI(const std::string &folder, const std::string &extensions) : Environment()
{
	this->fileName = "";
	this->isCGI = false;
	this->CGIFolder = folder;
	this->setCGIMapExtensions(extensions);
}

CGI::CGI(const CGI &src) : Environment()
{
	this->fileName = "";
	this->isCGI = false;
	this->CGIFolder = src.CGIFolder;
	this->CGIExtensions = src.CGIExtensions;
}

CGI::~CGI()
{
}

void CGI::setCGIFolder(const std::string &folder)
{
	this->CGIFolder = folder;
}

void CGI::setFileName(const std::string &Name, const std::string &Args)
{
	std::vector<std::string> tmp;

	fileName = Name;
	this->setArgs(Args);
	tmp = splitString(Args, '&');
}

void CGI::setIsCGI(bool valCGI)
{
	this->isCGI = valCGI;
}

bool CGI::setIdentifyCGIFromFileName(const std::string &str)
{
	if (str.find(CGIFolder) != std::string::npos)
	{
		isCGI = true;
	}
	else
	{
		isCGI = false;
	}
	return isCGI;
}

std::string CGI::getFileName()
{
	return fileName;
}

bool CGI::getIsCGI()
{
	return isCGI;
}

std::string CGI::getCGIFolder()
{
	return CGIFolder;
}

void CGI::setArgs(const std::string &str)
{
	std::vector<std::string> vec = splitString(str, '&');
	for (size_t i = 0; i < vec.size(); i++)
	{
		args.push_back(vec[i]);
	}

}

std::vector <ExtendedString> CGI::getArgs()
{
	return args;
}

std::map<std::string, std::string>::iterator CGI::findCGIExtension(const std::string &str) 
{
	return this->CGIExtensions.find(str); 
}

std::string CGI::getCGIExtension(const std::string &str) 
{
	return this->CGIExtensions[str]; 
}

std::string CGI::getFileExtension()
{
	std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
	return ext;
}

std::string CGI::execute()
{
	int timeout = 5;
	ExtendedString Executable = this->getFileName();
	Executable.replaceString("//", "/");
	signal(SIGALRM, &CGI::alarm_handler);
	int fd[2], tmp_fd;
	tmp_fd = dup(STDOUT_FILENO);
	std::vector<char*> ExecArray;
	ExecArray.push_back(const_cast<char *>(Executable.c_str()));
	std::vector<ExtendedString>::iterator itb = args.begin();
	std::vector<ExtendedString>::iterator ite = args.end();
	while (itb != ite) {
		ExecArray.push_back(const_cast<char *>(itb->c_str()));
		++itb;
	}
	ExecArray.push_back(NULL);
	if (pipe(fd) == -1) {
		throw INTERNAL_SERVER_ERROR_CODE;
	}
	pid_t pid = fork();
	if (pid == -1) {
		throw INTERNAL_SERVER_ERROR_CODE;
		return "";
	}
	if (pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		std::vector<char*> envp = this->getEnv();
		if (execve(Executable.c_str(), ExecArray.data(), envp.data()) == -1)
		{
			std::cerr << "Error: " << errno << std::endl;
			exit(errno);
		}
	} 
	close(fd[1]);
	CGI::ChildPID = pid;
	alarm(timeout);
	int status;
	waitpid(pid, &status, 0);
	std::string output;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
	{
		char buffer[1024];
		ssize_t bytesRead;
		while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0) 
		{
			output += std::string(buffer, bytesRead);
		}
		close(fd[0]);
		fd[1] = tmp_fd;
	}
	else
	{
		throw INTERNAL_SERVER_ERROR_CODE;
	}
	return output;
}

void CGI::setCGIMapExtensions(std::string const &cgi_extension)
{
	std::vector<std::string> aux;

	this->CGIExtensions.clear();
	if (cgi_extension.size() == 0)
		return;
	else if (cgi_extension.find(",") != std::string::npos)
	{
		aux = splitString(cgi_extension, ',');
	}
	else
	{
		aux.push_back(cgi_extension);
	}
	for (size_t i = 0; i < aux.size(); i++)
	{
		if (aux[i].size() == 0)
			continue;
		int n = count_chars(aux[i], ':');
		if (n > 1)
		{
			std::cerr << RED << "Error:: Wrong syntaxis " << RESET << aux[i] << std::endl;
			continue;
		}
		else if (n == 0)
		{
			this->CGIExtensions[aux[i]] = "";
		}
		else
		{
			std::vector<std::string> aux2 = splitString(aux[i], ':');
			this->CGIExtensions[aux2[0]] = aux2[1];
		}
	}
}

CGI *CGI::clone()
{
	return new CGI(*this);
}

int CGI::ChildPID = 0;

void CGI::alarm_handler(int sig)
{
	printLog("CGI", "Timeout reached");
	if (sig == SIGALRM && CGI::ChildPID > 0)
		kill(CGI::ChildPID, SIGKILL);
}