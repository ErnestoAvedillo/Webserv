/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileContent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:48:24 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/30 13:48:25 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <filesystem>
#include <vector>
#include <map>
#include <vector>
#include <map>
#include "colors.h"
#include "utils.hpp"
#include "CGI.hpp"
// #include "Server.hpp"
#include "ListDir.hpp"
#include "ExtendedString.hpp"
#include "StatusCode.hpp"

//#define MAX_SENT_BYTES 8192
class FileContent : public StatusCode
{
	protected:
		CGI *cgiModule;
	private:
		std::string fileName;
		std::string indexName;
		std::vector <std::string> args;
		ExtendedString content;
		std::ifstream file;
		bool isFistFragment;
		bool isFileOpen;
		bool sendComplete;
		bool isAutoIndex;
		std::string homeFolder;
		struct stat fileStat;
		long long startRange;
		long long currentSendingPosition;
		long long lastSendingPosition;
		ListDir *listDir;
		size_t completeContentSize;
		std::string splitFileFromArgs(const std::string &);
		bool FileOrFolerExtists(const std::string &);
		size_t endRange;
	public:
		FileContent();
		~FileContent();
		int openFile();
		bool setFileName(const std::string &, const std::string &);
		void setIsCGI(bool isCgi);
		void setStartRange(long long);
		void setEndRange(long long);
		long long getStartRange();
		std::string getFileName();
		std::string getContent();
		void setIsFileOpen(bool);
		void setIsSendComplete(bool);
		bool getIsSendComplete();
		void setIsCgi(bool );
		bool getIsCgi();

		bool getIsFileOpen();
		void setFirstFragment(bool);
		bool getFirstFragment();
		std::string getLastModified();
		size_t getCompleteContentSize();
		void setCompleteContentSize(size_t);
		bool isInputDirectory();
		void setRange(size_t );
		void setIsAutoIndex(bool);
		bool getIsAutoIndex();
		void setIndexName(const std::string &);
		std::string getIndexName();
		void setHomeFolder(const std::string &);
		std::string getHomeFolder();
		void setCGIModule(CGI *);
		long long getFileSize();
		long long getCurrentSendingPosition();
		long long getLastSendingPosition();
};
