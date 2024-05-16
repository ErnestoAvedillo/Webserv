/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:43:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/15 19:19:00 by jcheel-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
	std::vector<std::string> result;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter)) {
		if (token != "")
			result.push_back(token);
	}

	return result;
}
