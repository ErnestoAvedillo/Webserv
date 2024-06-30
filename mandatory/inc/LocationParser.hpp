#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Header.hpp"
#include "Server.hpp"
#include "StatusCodesDefinition.hpp"

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

class Server;

class LocationParser
{
	private:
		Header request;
		Header response;
		Server *server;
		Receive *receiver;
		std::string query;
		bool isAutoIndex;
		bool isCGI;
		size_t startRange;
		size_t endRange;
		std::string cgiPath;
		// std::string
		bool isCookie;
		std::vector<std::string> cookies;
		
		bool isSessionId;
		std::string sessionId;
	public:
		LocationParser(Header request, Server *server, Receive *receiver);
		~LocationParser();
		int matchingLocation();
		int isAllowedMethod(Location *location);
		Header getRequest();
		Header getResponse();
		bool getIsAutoIndex();
		bool getIsCGI();
		void checks();
		void setCookies();
		size_t getStartRange();
		size_t getEndRange();
		std::string getQuery();
		void setSessionId();
};