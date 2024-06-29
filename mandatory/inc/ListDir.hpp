/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 19:03:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/29 12:32:21 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <algorithm>
#include <dirent.h>
#include "Attributes.hpp"
#include "utils.hpp"
#include "colors.h"
#define TEMPLATE_LIST_DIR "./Template/dir_list.html"
class ListDir
{
    private:
        std::ifstream file;
        std::string path;
        std::string homePath;
        std::string contentToSend;
        std::size_t      posToSend;
        std::map<std::string, Attributes *> files;
        bool        isSendComplete;
    public:
        ListDir();
        ListDir(const std::string &, const std::string &);
        ~ListDir();
        void setListOfFiles();
        std::map <std::string, Attributes *>::iterator getBeginOfFiles();
        std::map <std::string, Attributes *>::iterator getEndOfFiles();
        std::string getDirFileList();
        void setContentToList();
        void setIsSendComlete();
        bool getIsSendComlete();
        void openMasterListFile();
        std::string getContentToSend();
        size_t getSizeToSend();
        void setSubdirectory(const std::string &);
};

