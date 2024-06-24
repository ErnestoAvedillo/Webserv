#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "utils.hpp"
#include "ExtendedString.hpp"
#include "StatusCodesDefinition.hpp"

class StatusCode {
private:
	std::map<int, std::string> mapCodes;
	std::map<int, ExtendedString> mapCodesFileContent;
	int currentCode;

public:
	StatusCode();
	~StatusCode();
	int getCode();
	void createDefaultErrorCodes();
	void setFileContentForStatusCode(int, const std::string &);
	std::string getCodeContent(int);
	ExtendedString getFileContentForStatusCode(int);
	void setCurrentCode(int);
	int getCurrentCode();
	void loadErrorPageFromDir(const ExtendedString &);
};