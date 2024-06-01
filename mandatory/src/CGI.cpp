/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/01 17:40:53 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

CGI::CGI()
{
	file_name = "";
}

CGI::CGI(const std::string& str)
{
	std::vector <std::string> tmp = splitString(str, '?');
	file_name = tmp[0];
	if (tmp.size() > 1)
		args = splitString(tmp[1], '&');
}

CGI::~CGI()
{
}

void CGI::setFileName(const std::string& str)
{
	file_name = str;
}

std::string CGI::getFileName()
{
	return file_name;
}

void CGI::setArgs(const std::vector <std::string>& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		args.push_back(vec[i]);
	}
}

std::vector <std::string> CGI::getArgs()
{
	return args;
}

std::string CGI::execute()
{
	int fd[2], tmp_fd;
	tmp_fd = dup(STDOUT_FILENO);
	std::vector<char*> argsArray;
	argsArray.push_back(const_cast<char *>(file_name.c_str()));
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
		if (execve(file_name.c_str(), argsArray.data(), NULL) == -1)
		{
			// Handle error executing file
			std::cerr << "Error executing file " << errno << std::endl;
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