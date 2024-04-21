/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itos.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:55:21 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/21 14:06:13 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>

std::string itos (int n)
{
	std::string str;
	std::stringstream ss;
	
	ss << n;
	ss >> str;

	return  str;
}