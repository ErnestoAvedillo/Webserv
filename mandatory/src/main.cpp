/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:21:58 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/04 14:16:00 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

int main(int av, char **ac)
{
		if (av != 2)
	{
		std::cerr << "Error: No se ha pasado el archivo de configuración" << std::endl;
		return 1;
	}
	WebServer webServer;
	webServer.loadConfigFile(ac[1]);
	webServer.launchServers();
}
