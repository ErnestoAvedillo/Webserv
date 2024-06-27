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
#define VAR_LOC_CGI_ENABLED "cgi_enabled"
#define VAR_LOC_CGI_EXTENSION "cgi_extension"
#define VAR_LOC_COOKIE "set-cookie"
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
		std::string cgiExtensionStr;
		std::vector<std::string> cgiExtension;
		std::string cookiesStr;
		std::vector<std::string> cookies;
		enum LocationType LocationType;
		std::string cgiEnabledStr;
		bool isGetAllowed;
		bool isPostAllowed;
		bool isDeleteAllowed;
		bool isCgi;
		bool isCookie;
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
		const std::string& getAutoIndexStr() const ;
		bool getAutoIndex() const;
		const std::string& getAlias() const ;
		enum LocationType getLocationType();
		bool getIsCgi() const;
		bool getIsCookie() const;
		std::vector<std::string>  getCookies() const;
		// Setter methods

		void setName(const std::string&);
		void setRoot(const std::string&);
		void setReturn(const std::string&);
		void setIndex(const std::string&);
		void setAllowMethodsStr(const std::string& );
		void setAllowMethods(const std::string& );
		void setAutoindex(const std::string&);
		void setAlias(const std::string&);
		void setCgiEnabledStr(const std::string &option);
		bool setCgiEnabled();
		void setCgiExtensionStr(const std::string &extensions);
		void setCgiExtension(const std::string &extensions);
		void setCookieStr(const std::string &cookie);
		void setCookies(const std::string &cookie);
		// Load data from a string configuration
		void print();
		void checkVariables(bool serverAutoIndex);
		std::vector<std::string> getCgiExtension();
};

//typedef void (Location::*location)(std::string);
//typedef std::map<std::string, location> location_methods_t;

#endif 
