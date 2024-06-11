/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/11 10:22:16 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

CGI::CGI()
{
	this->fileName = "";
	this->isCGI = false;
	this->CGIFolder = "/cgi/";
	this->setCGIMapExtensions("cgi");
}

CGI::CGI(const std::string &folder, const std::string &extensions)
{
	this->fileName = "";
	this->isCGI = false;
	this->CGIFolder = folder;
	this->setCGIMapExtensions(extensions);
}

CGI::CGI(const CGI &src)
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

void CGI::setFileName(const std::string& str)
{
	std::vector<std::string> tmp;

	if (str.find("?") != std::string::npos)
	{
		tmp = splitString(str, '?');
		fileName = tmp[0];
		std::cout << "File name1: " << fileName << std::endl;
		args = splitString(tmp[1], '&');
	}
	else
	{
		fileName = str;
	}
	std::map<std::string, std::string>::iterator it = this->findCGIExtension(this->getFileExtension());
	if (it != this->CGIExtensions.end())
	{
		if (it->second.size() != 0)
		{
			fileName =it->second;
			this->setArgs(tmp[1]);
		}
	}
}

bool CGI::setIsCGI(const std::string& str)
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

std::vector <std::string> CGI::getArgs()
{
	return args;
}

std::map<std::string, std::string>::iterator CGI::findCGIExtension(const std::string &str) { return this->CGIExtensions.find(str); }

std::string CGI::getCGIExtension(const std::string &str) { return this->CGIExtensions[str]; }

std::string CGI::getFileExtension()
{
	std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
	return ext;
}

std::string CGI::execute()
{
	int fd[2], tmp_fd;
	tmp_fd = dup(STDOUT_FILENO);
	std::vector<char*> argsArray;
	argsArray.push_back(const_cast<char *>(fileName.c_str()));
	std::vector<std::string>::iterator itb = args.begin();
	std::vector<std::string>::iterator ite = args.end();
	while (itb != ite) {
		argsArray.push_back(const_cast<char *>(itb->c_str()));
		++itb;
	}
	argsArray.push_back(NULL);
	if (pipe(fd) == -1) {
		throw std::runtime_error("Pipe error on creation");
	}
	pid_t pid = fork();
	if (pid == -1) {
		// Handle error forking process
		throw std::runtime_error("Fork error on creation");
		return "";
	}
	if (pid == 0) {
		// Child process
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		// Convert the arguments vector to a null-terminated array
		// Execute the file with its parameters
		if (execve(fileName.c_str(), argsArray.data(), NULL) == -1)
		{
			// Handle error executing file
			std::cerr << "Error executing file " << fileName << "  with errno " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
	} 
	// Parent process
	close(fd[1]);
	// Wait for the child process to finish
	int status;
	waitpid(pid, &status, 0);

	// Check if the child process exited normally
	if (WIFEXITED(status)) {
		// Read the output from the file descriptor
		char buffer[1024];
		std::string output;
		ssize_t bytesRead;
		while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0) {
			output += std::string(buffer, bytesRead);
		}
		close(fd[0]);
		fd[1] = tmp_fd;
		return output;
	} else {
		throw std::runtime_error("Child process exited abnormally");
	}
	return "";
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
			this->CGIExtensions[aux[i]] = ""; // Si no hay programa de ejecución, se asume que es un CGI ejecutable.
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