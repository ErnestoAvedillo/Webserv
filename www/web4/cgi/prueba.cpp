
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <errno.h>

int main(int av, char **ac) {
	// Replace "program" with the actual program you want to execute
	if(av < 2) {
		std::cerr << "Usage: " << ac[0] << " <program>" << std::endl;
		return 1;
	}
	int fd[2], tmp_fd;
	if (pipe(fd) == -1)
	{
		// Handle error creating pipe
		throw std::runtime_error("Pipe error on creation");
	}
	else
	{
		std::cout << "Pipe created" << std::endl;
	}
	tmp_fd = dup(STDOUT_FILENO);
	//char* args[] = {ac[1], NULL};
	std::string program = ac[1];
	std::vector<char *> args;
	args.push_back(ac[1]);
	for (int i = 2; i < av; i++)
	{
		args.push_back(ac[i]);
	}		
	int pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		//close(fd[1]);
		if(execve(ac[1], args.data(), NULL)==-1) {
			std::cerr << "Error: " << errno << std::endl;
			exit (-1);
		}
		std::cout << "Error: " << errno << std::endl;
	}
	close(fd[1]);
	// Parent process

	// Wait for the child process to finish
	int status;
	waitpid(pid, &status, 0);
	std::cout << "STATUS " << status << std::endl;
	// Check if the child process exited normally
	if (WIFEXITED(status))
	{
		// Read the output from the file descriptor
		std::cout << "Inside if " << std::endl;
		char buffer[1024];
		std::string output;
		ssize_t bytesRead;
		while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0)
		{
			std::cout << "Bytes read  " << bytesRead << std::endl;
			output += std::string(buffer, bytesRead);
		}
		close(fd[0]);
		fd[1] = tmp_fd;
		// Do something with the output
		std::cout << "SALIDA PROGRAMA " << output << std::endl;
		// ...
	}
	// execve only returns if an error occurs
	return 1;
}

void executeProgram(const std::string& program, const std::vector<char*>& args, int fd[2]) {
	int pid = fork();
	if (pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		if (execve(program.c_str(), args.data(), NULL) == -1) {
			std::cerr << "Error: " << errno << std::endl;
			exit(-1);
		}
	}
	close(fd[1]);
	// Parent process
	// Wait for the child process to finish
	int status;
	waitpid(pid, &status, 0);
	std::cout << "STATUS " << status << std::endl;
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
		// Do something with the output
		std::cout << "SALIDA PROGRAMA " << output << std::endl;
		// ...
	}
}