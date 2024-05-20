#include <map>
#include <string>


std::map<int, std::string> createHttpErrorCodes()
{
	std::map<int, std::string> errorTypes;
	
	// Add HTTP error types to the map
	errorTypes[400] = "Bad Request";
	errorTypes[401] = "Unauthorized";
	errorTypes[403] = "Forbidden";
	errorTypes[404] = "Not Found";
	errorTypes[500] = "Internal Server Error";
	// Add more error types as needed
	
	return errorTypes;
}

