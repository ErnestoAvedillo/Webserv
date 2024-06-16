/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:38:18 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/16 15:13:01 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"

std::map<std::string, int> var_names_location()
{
	std::map<std::string, int> varnames;
	varnames[VAR_LOC_NAME] = 0;
	varnames[VAR_LOC_ROOT] = 0;
	varnames[VAR_LOC_RETURN] = 0;
	varnames[VAR_LOC_INDEX] = 0;
	varnames[VAR_LOC_ALLOW_METHODS] = 0;
	varnames[VAR_LOC_AUTOINDEX] = 0;
	varnames[VAR_LOC_ALIAS] = 0;
	varnames[VAR_LOC_CGI_PATH] = 0;
	varnames[VAR_LOC_CGI_EXTENSION] = 0;
	return varnames;
}
// Method to get the methods of the location
std::map<std::string, void (Location::*)(const std::string&)> getLocationMethods()
{
	std::map<std::string, void (Location::*)(const std::string&)> locationMethods;
	
	locationMethods[VAR_LOC_NAME] = &Location::setName;
	locationMethods[VAR_LOC_ROOT] = &Location::setRoot;
	locationMethods[VAR_LOC_RETURN] = &Location::setReturn;
	locationMethods[VAR_LOC_INDEX] = &Location::setIndex;
	locationMethods[VAR_LOC_ALLOW_METHODS] = &Location::setAllowMethodsStr;
	locationMethods[VAR_LOC_AUTOINDEX] = &Location::setAutoindex;
	locationMethods[VAR_LOC_ALIAS] = &Location::setAlias;
	locationMethods[VAR_LOC_CGI_PATH] = &Location::setCgiPathStr;
	locationMethods[VAR_LOC_CGI_EXTENSION] = &Location::setCgiExtensionStr;
	return locationMethods;
}

// Default constructor
Location::Location() 
{
	name = "";
	root = "";
	return_ = "";
	index = "";
	allowMethodsStr = "";
	autoindex = "";
	alias = "";
	cgiPathStr = "";
	cgiExtensionStr = "";
	isCgi = false;
}
Location::Location(std::string const &content)
{
	// std::cout << "Location constructor" << std::endl;
	this->loadData(content);
}
// Copy constructor
Location::Location(const Location& other)
{
	setName(other.name);
	setRoot(other.root);
	setReturn(other.return_);
	setIndex(other.index);
	setAllowMethodsStr(other.allowMethodsStr);
	setAutoindex(other.autoindexStr);
	setAlias(other.alias);
}

// Destructor
Location::~Location() {}

// Getter methods
const std::string &Location::getName() const { return name; }
const std::string &Location::getRoot() const { return root; }
const std::string &Location::getReturn() const { return return_; }
const std::string &Location::getIndex() const { return index; }
const std::string &Location::getAllowMethodsStr() const { return allowMethodsStr; }
const std::string &Location::getAutoindex() const { return autoindexStr; }
const std::string &Location::getAlias() const { return alias; }
bool Location::getGetAllowed() const { return isGetAllowed; }
bool Location::getPostAllowed() const { return isPostAllowed; }
bool Location::getDeleteAllowed() const { return isDeleteAllowed; }
enum LocationType Location::getLocationType() { return this->LocationType; }
// Setter methods
void Location::setName(const std::string &n) { name = n; }
void Location::setRoot(const std::string &r) { root = r; }
void Location::setReturn(const std::string &ret) { return_ = ret; }
void Location::setIndex(const std::string &idx) { index = idx; }
void Location::setAllowMethodsStr(const std::string &allow) { allowMethodsStr = allow; }
void Location::setAutoindex(const std::string &autoidx) { autoindexStr = autoidx; }
void Location::setAlias(const std::string &als) { alias = als; }
void Location::setCgiPathStr(const std::string &paths) { this->cgiPathStr = paths; }
void Location::setCgiExtensionStr(const std::string &extensions) { this->cgiExtensionStr = extensions; }

void Location::setAllowMethods(const std::string& methods)
{
	std::string line;
	std::istringstream allowMethodsStream(methods);
	while (std::getline(allowMethodsStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		if (line == "GET")// || line == "POST" || line == "DELETE")
		{
			this->allowMethods.push_back(line);
			this->isGetAllowed = true;
		}
		else if (line == "POST")
		{
			this->allowMethods.push_back(line);
			this->isPostAllowed = true;
		}
		else if (line == "DELETE")
		{
			this->allowMethods.push_back(line);
			this->isDeleteAllowed = true;
		}
		else
		{
			printLog("ERROR", "allow_method " + line + " is not valid.");
			exit(1);
		}
	}
}
void Location ::setCgiPath(const std::string &paths)
{
	std::string line;
	std::istringstream cgiPathStream(paths);
	while (std::getline(cgiPathStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		this->cgiPath.push_back(line);
	}
}

void Location ::setCgiExtension(const std::string &extensions)
{
	std::string line;
	std::istringstream cgiExtStream(extensions);
	while (std::getline(cgiExtStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		this->cgiExtension.push_back(line);
	}
}
// Load data from a string configuration
int Location::loadData(const std::string &content)
{
	std::string start_string = STR_START;
	std::string line;
	std::string straux;
	std::map<std::string, int> varnames = var_names_location();

	std::istringstream fileContentStream(content);
	while (std::getline(fileContentStream, line, ';'))
	{
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		if (line == "}")
			continue;
		std::map<std::string, int>::iterator it = varnames.begin();
		while (it != varnames.end())
		{
			if (line.substr(0 , line.find(":") ) == it->first)
			{
				if (it->second == 1)
					std::cerr << "Error: duplicated variable " << it->first << std::endl;
				it->second = 1;
				break;
			}
			it++;
		}
		if (line.length() == 0 || line == "}" || line == "{")
			continue;
		else if (it == varnames.end())
			printLog("ERROR", "Unrecognized variable " + line.substr(0, line.find(":")));
		else
		{
			straux = line.substr(line.find(":") + 1, line.size());
			(this->*getLocationMethods()[it->first])(straux);
		}
	}
	return 0;
}

void Location::print()
{
	std::cout << "-------------------- Location --------------------" << std::endl;
	std::cout << "Name: " << name << std::endl;
	std::cout << "Root: " << root << std::endl;
	std::cout << "Return: " << return_ << std::endl;
	std::cout << "Index: " << index << std::endl;
	std::cout << "Allow Methods: " << allowMethodsStr << std::endl;
	std::cout << "Autoindex: " << autoindex << std::endl;
	std::cout << "Alias: " << alias << std::endl;
	std::cout << "Cgi Path: " << cgiPathStr << std::endl;
	std::cout << "Cgi Extension: " << cgiExtensionStr << std::endl;
}

// location:{                   
//       name:/tours;
//       root:docs/fusion_web;           # root folder of the location, if not specified, taken from the server. 
//                                       # EX: - URI /tours           --> docs/fusion_web/tours
//                                       #     - URI /tours/page.html --> docs/fusion_web/tours/page.html 
//       autoindex:on;                   # turn on/off directory listing
//       allow_methods: POST,GET;         # allowed methods in location, GET only by default
//       index:index.html;               # default page when requesting a directory, copies root index by default
//       return:abc/index1.html;         # redirection
//       alias: docs/fusion_web;         # replaces location part of URI. 
//                                       # EX: - URI /tours           --> docs/fusion_web
//                                       #     - URI /tours/page.html --> docs/fusion_web/page.html 
//   }

// bool isValidUrl(const std::string& url)
// {
// 	// Check if the URL starts with a valid scheme
// 	std::string validSchemes[] = {"http://", "https://", "ftp://"};
// 	bool validScheme = false;
// 	for (const std::string& scheme : validSchemes) {
// 		if (url.substr(0, scheme.length()) == scheme) {
// 			validScheme = true;
// 			break;
// 		}
// 	}
// 	if (!validScheme) {
// 		return false;
// 	}

// 	// Check if the URL contains only valid characters
// 	std::string validCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";
// 	for (char c : url) {
// 		if (validCharacters.find(c) == std::string::npos) {
// 			return false;
// 		}
// 	}

// 	return true;
// }

// #include <string>
// #include <algorithm>

// bool isValidUrl(const std::string& url)
// {
//     // Check if the URL starts with a valid scheme
//     std::string validSchemes[] = {"http://", "https://", "ftp://"};
//     bool validScheme = false;
//     for (int i = 0; i < 3; ++i) {
//         if (url.substr(0, validSchemes[i].length()) == validSchemes[i]) {
//             validScheme = true;
//             break;
//         }
//     }
//     if (!validScheme) {
//         return false;
//     }

//     // Check if the URL contains only valid characters
//     std::string validCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";
//     for (std::string::const_iterator it = url.begin(); it != url.end(); ++it) {
//         if (validCharacters.find(*it) == std::string::npos) {
//             return false;
//         }
//     }

//     return true;
// }

void Location::checkVariables()
{
	switch (Parser::checkLocationName(this->name))
	{
		case 2:
			if (!Parser::checkCgiString(this->cgiPathStr, this->cgiExtensionStr))
				exit(1);
			else
			{
				this->setCgiPath(this->cgiPathStr);
				this->setCgiExtension(this->cgiExtensionStr);
				Parser::checkCgi(this->cgiPath, this->cgiExtension);
				this->isCgi = true;
			}
			break ;
		case 1:
			break;
	}
	
	switch (Parser::checkRootAliasReturn(this->root, this->alias,this->return_))
	{
		case ROOT:
			LocationType = ROOT;
			break ;
		case ALIAS:
			LocationType = ALIAS;
			break ;
		case RETURN:
			if (this->isCgi)
			{
				printLog("ERROR", "Cgi location cannot have return");
				exit(1);
			}
			Parser::checkReturnIgnore(this->allowMethodsStr, this->autoindexStr, this->index);
			LocationType = RETURN;
			break ;
		default:
			break;
	}

	switch (Parser::checkAutoIndex(this->getAutoindex()))
	{
		case true:
			this->autoindex = true;
			break;
		case false:
			this->autoindex = false;
			break;
	}

	this->setAllowMethods(this->allowMethodsStr);
	if (!Parser::checkAllowedMethods(this->allowMethodsStr))
		this->isGetAllowed = true;
	if (LocationType != RETURN)
		Parser::checkIndex(this->getIndex(), this->getRoot());
	
}