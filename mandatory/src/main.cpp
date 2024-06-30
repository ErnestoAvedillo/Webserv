/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:51:34 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 15:21:02 by eavedill         ###   ########.fr       */
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
