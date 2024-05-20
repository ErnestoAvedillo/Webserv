/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toString.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:55:21 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/20 13:50:25 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>

template <typename T>
std::string toString (T n)
{
	std::string str;
	std::stringstream ss;
	
	ss << n;
	ss >> str;

	return  str;
}
