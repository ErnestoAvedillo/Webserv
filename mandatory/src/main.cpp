/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/08 02:34:47 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServer.hpp"

int main(int av, char **ac)
{
	if (av != 2)
	{
		std::cerr << "usage: ./Webserv <filename>" << std::endl;
		return 1;
	}
	WebServer webServer;
	webServer.loadConfigFile(ac[1]);
	if (!webServer.checkSyntax())
		return 1;

	// webServer.initalizer();

	webServer.processConfigFile();
	webServer.parseInfo();
	webServer.launchServers();
	return 0;
}
