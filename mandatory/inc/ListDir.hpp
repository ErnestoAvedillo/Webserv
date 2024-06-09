/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 19:03:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/09 14:27:35 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <dirent.h>
#include "Attributes.hpp"
#include "utils.hpp"

class ListDir
{
    private:
        std::ifstream file;
        std::string path;
        std::map <std::string, Attributes *> files;
        bool isSendComplete;
    public:
        ListDir();
        ListDir(const std::string &);
        ~ListDir();
        void setListOfFiles(const std::string &);
        std::map <std::string, Attributes *>::iterator getBeginOfFiles();
        std::map <std::string, Attributes *>::iterator getEndOfFiles();
        std::string getDirFileList();
        std::string getContentToList();
        bool getsIsSendComlete();
};

