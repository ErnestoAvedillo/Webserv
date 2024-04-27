/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:05:11 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 18:42:10 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <fstream>
# include <iostream>
# include "Server.hpp"
# include "commonFunctions.hpp"

class WebServer {
	private:
		std::vector<Server *> servers;
		std::ifstream		configFile;
		std::string			configFilename;
		std::string 		fileContent;
		void processConfigFile();

	public:
		WebServer();
		~WebServer();
		// WebServer(WebServer const &copy);
		// WebServer &operator=(WebServer const &copy);
		void loadConfigFile(std::string configFile);

};
#endif
