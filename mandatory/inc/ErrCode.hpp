#ifndef ERRCODE_HPP
#define ERRCODE_HPP

#include "../inc/main.hpp"

class ErrCode {
public:
	ErrCode();
	std::string getErrorMessage(int) const;

private:
	std::map<int, std::string> errorCodes;
};

#endif // ERRCODE_HPP