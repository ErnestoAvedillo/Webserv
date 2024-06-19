#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Header.hpp"
#include "Server.hpp"


enum LocationStatus
{
	/* Correct */
	NO_LOCATION,
	OK,
	NOT_ALLOWED,
	NOT_FOUND,
	NOT_IMPLEMENTED,
	REQUEST_ENTITY_TOO_LARGE,
	REQUEST_URI_TOO_LONG,
	INTERNAL_SERVER_ERROR,
	NOT_MODIFIED,
	REDIRECT,

};

class LocationParser
{
	private:
		Header request;
		Header response;
		Server *server;
		// FileContent *fileContent;
		Receive *receiver;
		std::string path;
		// std::string
	public:
		LocationParser(Header request, Server *server, Receive *receiver);
		~LocationParser();
		int matchingLocation();
		int isAllowedMethod(Location *location);
		Header getRequest();
		Header getResponse();

};