/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/26 11:10:15 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

CGI::CGI()
{
	std::cout << "CGI defaut constructor" << std::endl;
	file_name = "";
}

CGI::CGI(const std::string& str)
{
	std::cout << "CGI constructor" << std::endl;
	std::vector <std::string> tmp = splitString(str, '?');
	file_name = tmp[0];
	if (tmp.size() > 1)
		args = splitString(tmp[1], '&');
}

CGI::~CGI()
{
	std::cout << "CGI destructor" << std::endl;
}

std::string CGI::execute()
{
	int fd[2], tmp_fd;
	if (pipe(fd) == -1) {
		// Handle error creating pipe
		throw std::runtime_error("Pipe error on creation");
	}
	tmp_fd = dup(STDOUT_FILENO);
	// Create a child process using fork
	pid_t pid = fork();
	if (pid == -1) {
		// Handle error forking process
		throw std::runtime_error("Fork error on creation");
	}

	if (pid == 0) {
		// Child process
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		// Convert the arguments vector to a null-terminated array
		std::vector<char*> argsArray;
		std::vector<std::string>::iterator itb = args.begin();
		std::vector<std::string>::iterator ite = args.end();
		while (itb != ite) {
			argsArray.push_back(const_cast<char*>(itb->c_str()));
			++itb;
		}
		argsArray.push_back(nullptr);

		// Execute the file with its parameters
		if (execve(file_name.c_str(), argsArray.data(), nullptr) == -1) {
			// Handle error executing file
			exit(EXIT_FAILURE);
		}
	} else {
		close(fd[1]);
		close(fd[0]);
		// Parent process

		// Wait for the child process to finish
		int status;
		waitpid(pid, &status, 0);

		// Check if the child process exited normally
		if (WIFEXITED(status)) {
			// Read the output from the file descriptor
			char buffer[1024];
			std::string output;
			ssize_t bytesRead;
			while ((bytesRead = read(fd[1], buffer, sizeof(buffer))) > 0) {
				output += std::string(buffer, bytesRead);
			}
			fd[1] = tmp_fd;
			// Do something with the output
			return output;
			// ...
		} else {
			throw std::runtime_error("Child process exited abnormally");
		}
	}
	return "";
}