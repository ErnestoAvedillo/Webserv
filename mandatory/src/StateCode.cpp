#include "../inc/StateCode.hpp"

StateCode::StateCode()
{
	this->createDefaultErrorCodes();
	this->setFileContentForStateCode(0, DEFAULT_ERROR_FILE);
}

StateCode::~StateCode()
{
}

void StateCode::createDefaultErrorCodes(){
	// Add HTTP error types to the map
	this->mapCodes[0] = "000 Template";
	this->mapCodes[100] = "100 Continue";
	this->mapCodes[101] = "101 Switching Protocols";
	this->mapCodes[200] = "200 OK";
	this->mapCodes[201] = "201 Created";
	this->mapCodes[202] = "202 Accepted";
	this->mapCodes[203] = "203 Non - Authoritative Information";
	this->mapCodes[204] = "204 No Content";
	this->mapCodes[205] = "205 Reset Content";
	this->mapCodes[206] = "206 Partial Content";
	this->mapCodes[300] = "300 Multiple Choices";
	this->mapCodes[301] = "301 Moved Permanently";
	this->mapCodes[302] = "302 Found";
	this->mapCodes[303] = "303 See Other";
	this->mapCodes[304] = "304 Not Modified";
	this->mapCodes[305] = "305 Use Proxy";
	this->mapCodes[307] = "307 Temporary Redirect";
	this->mapCodes[400] = "400 Bad Request";
	this->mapCodes[401] = "401 Unauthorized";
	this->mapCodes[402] = "402 Payment Required";
	this->mapCodes[403] = "403 Forbidden";
	this->mapCodes[404] = "404 Not Found";
	this->mapCodes[405] = "405 Method Not Allowed";
	this->mapCodes[406] = "406 Not Acceptable";
	this->mapCodes[407] = "407 Proxy Authentication Required";
	this->mapCodes[408] = "408 Request Time - out";
	this->mapCodes[409] = "409 Conflict";
	this->mapCodes[410] = "410 Gone";
	this->mapCodes[411] = "411 Length Required";
	this->mapCodes[412] = "412 Precondition Failed";
	this->mapCodes[413] = "413 Request Entity Too Large";
	this->mapCodes[414] = "414 Request - URI Too Large";
	this->mapCodes[415] = "415 Unsupported Media Type";
	this->mapCodes[416] = "416 Requested range not satisfiable";
	this->mapCodes[417] = "417 Expectation Failed";
	this->mapCodes[500] = "500 Internal Server Error";
	this->mapCodes[501] = "501 Not Implemented";
	this->mapCodes[502] = "502 Bad Gateway";
	this->mapCodes[503] = "503 Service Unavailable";
	this->mapCodes[504] = "504 Gateway Time - out";
	this->mapCodes[505] = "505 HTTP Version not supported";
}

void StateCode::setFileContentForStateCode(int CodeNumber, const std::string &CodeFileContent)
{
	ExtendedString content("");
	std::ifstream fileStream;
	fileStream.open(CodeFileContent.c_str(), std::ios::out | std::ios::binary); //| std::ios::app | std::ios::ate

	if(fileStream.is_open())
	{
		char buffer[MAX_SENT_BYTES];
		while (fileStream.read(buffer, MAX_SENT_BYTES))
			content.append(buffer, fileStream.gcount());
		content.append(buffer, fileStream.gcount());
	}
	else
	{
		printLog("WARNING", "trying to set an Error Page with a non existing file.\t Set content page to default " CHR_GREEN "GET" RESET " value");
	}

	mapCodesFileContent[CodeNumber] = content;
}

std::string StateCode::getCodeContent(int errorCode)
{
	this->setCurrentCode(errorCode);
	return mapCodes[errorCode];
}

ExtendedString StateCode::getFileContentForStateCode(int errorCode)
{
	this->setCurrentCode(errorCode);
	ExtendedString content;
	if (mapCodesFileContent.find(errorCode) == mapCodesFileContent.end())
	{
		content = mapCodesFileContent[0];
		content.replaceString("{{ErrorCode}}", mapCodes[errorCode]);
		return content;
	}
	return mapCodesFileContent[errorCode];
}

void StateCode::setCurrentCode(int code)
{
	this->currentCode = code;
}

int StateCode::getCurrentCode()
{
	return this->currentCode;
}

void StateCode::loadErrorPageFromDir(const ExtendedString &dir)
{
	struct dirent *dirp;
	DIR *dp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		throw std::runtime_error("Error opening directory");
	}
	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_type == DT_REG)
		{
			std::string fileName = dir + "/" + dirp->d_name;
			std::string code = dirp->d_name;
			code = code.substr(0, code.find("."));
			setFileContentForStateCode(atoi(code.c_str()), fileName);
		}
	}
	closedir(dp);
}