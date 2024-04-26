/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 07:30:34 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/26 16:22:22 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SENDFILE_HPP
#define SENDFILE_HPP

#include "../inc/main.hpp"

class FileName {
private:
	int Err_code;
	std::string filename;
ublic :
		// Constructor
		FileName();
	FileName(std::string filename);

	// Getter and Setter methods for Error and filename

	int getErrCode() const;
	std::string getErrDesc();
	void setErrCode();

	std::string getFilename() const;
	void setFilename(const std::string& filename);
	std::string retFileContent();

};
#endif // SENDFILE_HPP
