#pragma once
# include <string>
# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>
# include "Server.hpp"
# include "Location.hpp"

class WebServer {
	private:
		std::vector<Server>	servers;
		std::ifstream		configFile;
		std::string			configFilename;
		std::string 		fileContent;

	public:
		WebServer();
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer &operator=(WebServer const &copy);

		void loadConfigFile(std::string configFile);
		void processConfigFile(std::string fileContent);
		// void Init();
		// void Run();
		// void Stop();
};

