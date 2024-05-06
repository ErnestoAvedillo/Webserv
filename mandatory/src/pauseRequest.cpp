/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pauseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 13:02:02 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/05 14:08:35 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

void pauseRequest(std::string message)
{
	std::cout << message << std::endl;
	std::cin.get();
}
