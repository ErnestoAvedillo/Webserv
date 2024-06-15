#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <map>
#include <vector>
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


enum LocationType {
	ROOT=1,
	ALIAS,
	RETURN,
};

class Location 
{
	private:
		std::string name;
		std::string root;
		std::string return_;
		std::string index;
		std::string allowMethodsStr;
		std::vector<std::string> allowMethods; 
		std::string autoindexStr;
		bool autoindex;
		std::string alias;
		std::string cgiPathStr;
		std::vector<std::string> cgiPath;
		std::string cgiExtensionStr;
		std::vector<std::string> cgiExtension;
		LocationType locationType;
		bool isGetAllowed;
		bool isPostAllowed;
		bool isDeleteAllowed;
		bool isCgi;
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
		const std::string& getAllowMethodsStr() const;
		bool getGetAllowed() const;
		bool getPostAllowed() const;
		bool getDeleteAllowed() const;
		const std::string& getAutoindex() const ;
		const std::string& getAlias() const ;
		enum LocationType getLocationType();
		// Setter methods
		void setName(const std::string&);
		void setRoot(const std::string&);
		void setReturn(const std::string&);
		void setIndex(const std::string&);
		void setAllowMethodsStr(const std::string& );
		void setAllowMethods(const std::string& );
		void setAutoindex(const std::string&);
		void setAlias(const std::string&);
		void setCgiPathStr(const std::string &paths);
		void setCgiPath(const std::string &paths);
		void setCgiExtensionStr(const std::string &extensions);
		void setCgiExtension(const std::string &extensions);
		// Load data from a string configuration
		void print();
		void checkVariables();
};

//typedef void (Location::*location)(std::string);
//typedef std::map<std::string, location> location_methods_t;

#endif 
