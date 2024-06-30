/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:38:18 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 15:24:13 by eavedill         ###   ########.fr       */
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
	varnames[VAR_LOC_CGI_ENABLED] = 0;
	varnames[VAR_LOC_COOKIE] = 0;
	varnames[VAR_LOC_CGI_EXTENSION] = 0;
	varnames[VAR_LOC_SESSION_ID] = 0;
	return varnames;
}

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
	locationMethods[VAR_LOC_CGI_ENABLED] = &Location::setCgiEnabledStr;
	locationMethods[VAR_LOC_COOKIE] = &Location::setCookieStr;
	locationMethods[VAR_LOC_CGI_EXTENSION] = &Location::setCgiExtensionStr;
	locationMethods[VAR_LOC_SESSION_ID] = &Location::setSessionIdStr;
	return locationMethods;
}

Location::Location() 
{
	name = "";
	root = "";
	return_ = "";
	index = "";
	allowMethodsStr = "";
	autoindex = "";
	alias = "";
	cgiExtensionStr = "";
	isCgi = false;
	isSessionId = false;
	isCookie = false;
	cookiesStr = "";
}

Location::Location(std::string const &content)
{
	this->isCgi = false;
	this->cgiEnabledStr = "";
	isCookie = false;
	cookiesStr = "";
	isSessionId = false;
	this->loadData(content);
}

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

Location::~Location() {}

const std::string &Location::getName() const { return name; }
const std::string &Location::getRoot() const { return root; }
const std::string &Location::getReturn() const { return return_; }
const std::string &Location::getIndex() const { return index; }
const std::string &Location::getAllowMethodsStr() const { return allowMethodsStr; }
const std::string &Location::getAutoIndexStr() const { return autoindexStr; }
bool Location::getAutoIndex() const { return autoindex; }
const std::string &Location::getAlias() const { return alias; }
bool Location::getGetAllowed() const { return isGetAllowed; }
bool Location::getPostAllowed() const { return isPostAllowed; }
bool Location::getDeleteAllowed() const { return isDeleteAllowed; }
enum LocationType Location::getLocationType() { return this->LocationType; }
bool Location::getIsCgi() const { return isCgi; }
bool Location::getIsCookie() const { return isCookie; }
std::vector<std::string>  Location::getCookies() const { return cookies; }
void Location::setName(const std::string &n) { name = n; }
void Location::setRoot(const std::string &r) { root = r; }
void Location::setReturn(const std::string &ret) { return_ = ret; }
void Location::setIndex(const std::string &idx) { index = idx; }
void Location::setAllowMethodsStr(const std::string &allow) { allowMethodsStr = allow; }
void Location::setAutoindex(const std::string &autoidx) { autoindexStr = autoidx; }
void Location::setAlias(const std::string &als) { alias = als; }
void Location::setCgiEnabledStr(const std::string &cgiEnabled) {this->cgiEnabledStr = cgiEnabled;}
void Location::setSessionIdStr(const std::string &name){ this->isSessionId = true ; this->sessionIdStr = name; }
void Location::setCookieStr(const std::string &cookie) {this->isCookie = true ; this->cookiesStr = cookie; }

bool Location::setCgiEnabled()
{
	if (this->cgiEnabledStr == "true")
		isCgi = true;
	else if (cgiEnabledStr == "false")
		isCgi = false;
	else if (!this->cgiEnabledStr.empty() && (this->cgiEnabledStr != "true" || this->cgiEnabledStr != "false"))
	{
		printLog("WARNING", "cgi_enabled\t\tis not defined correctly. Set to default " CHR_GREEN "false" RESET);
		isCgi = false;
	}
	else if (this->cgiEnabledStr.empty())
		isCgi = false;
	return isCgi;
}

void Location::setCgiExtensionStr(const std::string &extensions) { this->cgiExtensionStr = extensions; }

void Location::setAllowMethods(const std::string& methods)
{
	std::string line;
	std::istringstream allowMethodsStream(methods);
	while (std::getline(allowMethodsStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		if (line == "GET")
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

void Location ::setCgiExtension(const std::string &extensions)
{
	std::string line;
	std::istringstream cgiExtStream(extensions);
	while (std::getline(cgiExtStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		if (line[0] == '.')
			line = line.substr(1, line.size());
		this->cgiExtension.push_back(line);
	}
}

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
			printLog("ERROR", "Unrecognized Location variable " + line.substr(0, line.find(":")));
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
	std::cout << "Cgi Extension: " << cgiExtensionStr << std::endl;
}

void Location::setCookies(const std::string &cookie)
{
	std::string line;
	std::istringstream cookieStream(cookie);
	while (std::getline(cookieStream, line, ','))
	{
		if (line.length() == 0)
			continue;
		this->cookies.push_back(line);
	}
}


void Location::checkVariables(bool serverAutoIndex)
{
	Parser::checkLocationName(this->name);
	if (this->setCgiEnabled())
	{
		this->setCgiExtension(this->cgiExtensionStr);
		if (!Parser::checkCgi(this->cgiExtensionStr))
			exit(1);
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
	switch (Parser::checkAutoIndex(this->getAutoIndexStr()))
	{
		case true:
			this->autoindex = true;
			if (this->autoindex == true && !this->index.empty())
				printLog("WARNING", "autoindex\t\twon't be used because index file is set");
			break;
		case false:
			if (this->getAutoIndexStr().empty())
			{
				printLog("NOTICE", "autoindex\t\tis not defined. Set to server value " CHR_GREEN + std::string(serverAutoIndex ? "on" : "off") + RESET);
				this->autoindex = serverAutoIndex;
			}
			else
				this->autoindex = false;
			break;
	}
	this->setAllowMethods(this->allowMethodsStr);
	if (!Parser::checkAllowedMethods(this->allowMethodsStr))
		this->isGetAllowed = true;
	if (LocationType == ROOT)
		Parser::checkIndex(this->getIndex(), this->getRoot() + "/" + this->getName());
	else if (LocationType == ALIAS)
		Parser::checkIndex(this->getIndex(), this->getAlias());

	if (this->isCookie && this->cookiesStr.empty())
	{
		printLog("WARNING", "cookie\t\t\tnot defined.");
		this->isCookie = false;
	}
	else if (this->isCookie && !this->cookiesStr.empty())
		this->setCookies(this->cookiesStr);
}

std::vector<std::string> Location::getCgiExtension()
{
	return this->cgiExtension;
}
	