/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 07:30:34 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/26 20:19:53 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SENDFILE_HPP
#define SENDFILE_HPP

// #include "../inc/main.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <sys/stat.h>

class FileName {
private:
	int Err_code;
	std::string filename;
public :
		// Constructor
	FileName();
	FileName(FileName const &src);
	FileName(std::string filename);
	FileName &operator=(FileName const &src);
	~FileName();

	// Getter and Setter methods for Error and filename

	int getErrCode() const;
	std::string getErrDesc();
	void setErrCode();

	std::string getFilename() const;
	void setFilename( std::string const & filename);
	std::string retFileContent();

};
#endif // SENDFILE_HPP

std::map<int, std::string> getHttpErrorTypes();
