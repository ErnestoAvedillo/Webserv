/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:48:49 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 14:43:58 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		Header						request;
		Header						response;
		Server						*server;
		Receive						*receiver;
		std::vector<std::string>	cookies;
		std::string					query;
		bool						isAutoIndex;
		bool						isCGI;
		bool						isCookie;
		size_t 						startRange;
		size_t 						endRange;
		std::string					cgiPath;
		
	public:
		LocationParser(Header request, Server *server, Receive *receiver);
		~LocationParser();
		int			matchingLocation();
		int			isAllowedMethod(Location *location);
		Header		getRequest();
		Header		getResponse();
		bool		getIsAutoIndex();
		bool		getIsCGI();
		void		checks();
		void		setCookies();
		size_t		getStartRange();
		size_t		getEndRange();
		std::string	getQuery();
};