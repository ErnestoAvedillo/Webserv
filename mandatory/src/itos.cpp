/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itos.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:55:21 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/21 14:10:18 by eavedill         ###   ########.fr       */
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

<<<<<<< HEAD
	return  str;	
=======
	return  str;
>>>>>>> 6887cbd6818abf438650f3c8864235c0ad16fa84
}
