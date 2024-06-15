/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/15 00:29:16 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/WebServer.hpp"

int main(int av, char **ac)
{
	if (av != 2)
	{
		std::cerr << CHR_YELLOW"usage: " << CHR_MGENTA"./Webserv <filename>" RESET<< std::endl;
		return 1;
	}
	WebServer webServer;
	webServer.loadConfigFile(ac[1]);
	if (!webServer.checkSyntax())
		return 1;

	webServer.processConfigFile();
	if (!webServer.parseInfo())
		return 1;
	webServer.launchServers();
	return 0;
}
