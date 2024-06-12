#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Parser.hpp"
#define VAR_LOC_NAME "name"
#define VAR_LOC_ROOT "root"
#define VAR_LOC_RETURN "return"
#define VAR_LOC_INDEX "index"
#define VAR_LOC_ALLOW_METHODS "allow_methods"
#define VAR_LOC_AUTOINDEX "autoindex"
#define VAR_LOC_ALIAS "alias"
#define VAR_LOC_CGI_PATH "cgi_path"
#define VAR_LOC_CGI_EXTENSION "cgi_ext"
#define STR_START "location:{"
class Location 
{
	private:
		std::string name;
		std::string root;
		std::string return_;
		std::string index;
		std::string allowMethodsStr;
		std::vector<std::string> allowMethods; 
		std::string autoindex;
		std::string alias;
		std::string cgiPath;
		std::string cgiExtension;
		int loadData(const std::string &data);

	public:
		// Default constructor
		Location();
		// constructor from content string "location :{}"
		Location(std::string const &);

		// Copy constructor
		Location(const Location& other);

		// Destructor
		~Location();

		// Getter methods
		const std::string& getName() const ;
		const std::string& getRoot() const ;
		const std::string& getReturn() const ;
		const std::string& getIndex() const;
		const std::string& getAllowMethods() const;
		const std::string& getAutoindex() const ;
		const std::string& getAlias() const ;

		// Setter methods
		void setName(const std::string&);
		void setRoot(const std::string&);
		void setReturn(const std::string&);
		void setIndex(const std::string&);
		void setAllowMethods(const std::string& );
		void setAutoindex(const std::string&);
		void setAlias(const std::string&);
		void setCgiPath(const std::string &cgi_path);
		void setCgiExtension(const std::string &cgi_ext);
		// Load data from a string configuration
		void print();
		void checkVariables();
};

//typedef void (Location::*location)(std::string);
//typedef std::map<std::string, location> location_methods_t;

#endif 
