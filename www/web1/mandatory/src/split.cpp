/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:43:57 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/27 18:34:57 by eavedill         ###   ########.fr       */
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
		result.push_back(token);
	}

	return result;
}
