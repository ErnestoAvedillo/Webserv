#pragma once
# include <map>
# include <string>
# include <sstream>
# include <iostream>

class Request{
	private:
		/* HTTP Request Line Attributes*/
		std::string							Method;
		std::string							Path;	
		std::string							Protocol;
		std::map<std::string, std::string>	Header;
	public:
		/* Default Constructor */
		Request();
		/* Load Constructor */
		/* @note only process the first line of the header*/
		//Request(std::string &requestLine);

		/* Copy Constructor */
		Request(const Request &copy);
		/* Copy Assignment operator */
		Request& operator=(const Request &copy);
		
		/* Destructor */
		~Request();

		// std::string	read
		void loadRequestLine(const std::string requestHeader);
		void loadLine(const std::string &requestLine); 
		void loadCompleteRequest(const std::string &requestHeader);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getProtocol() const;
		std::map<std::string, std::string> getHeader() const;

		void	setMethod(std::string method) ;
		void	setPath(std::string path);
		void	setProtocol(std::string protocol);
		void	setHeader(std::map<std::string, std::string> header);
};
