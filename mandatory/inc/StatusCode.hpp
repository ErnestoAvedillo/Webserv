/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:49:24 by eavedill          #+#    #+#             */
/*   Updated: 2024/07/04 07:19:29 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	ExtendedString getInternContent();
};