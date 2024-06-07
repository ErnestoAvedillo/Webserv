/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 19:03:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/06/07 19:41:20 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <dirent.h>
#include "Attributes.hpp"

class ListDir
{
    private:
        std::string path;
        std::map <std::string, Attributes *> files;
    public:
        ListDir();
        ListDir(const std::string &);
        ~ListDir();
        void setListOfFiles(const std::string &);
        std::map <std::string, Attributes *> getBeginOfFiles();
        std::map <std::string, Attributes *> getEndOfFiles();
};

