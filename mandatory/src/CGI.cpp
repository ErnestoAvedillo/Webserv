/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:42:08 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/25 23:16:25 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

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
	// Create a child process using fork
	pid_t pid = fork();

	if (pid == -1) {
		// Handle error forking process
		throw std::runtime_error("Fork error on creation");
	}

	if (pid == 0) {
		// Child process

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
			while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
				output += std::string(buffer, bytesRead);
			}

			// Do something with the output
			return output;
			// ...
		} else {
			throw std::runtime_error("Child process exited abnormally");
		}
	}
	return "";
}