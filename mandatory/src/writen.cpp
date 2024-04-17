/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 23:26:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/18 00:05:35 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwriten;
	const char *ptr;

	ptr = (char *) vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ((nwriten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwriten < 0 && errno == EINTR)
				nwriten = 0;
			else
				return -1;
		}
		nleft -= nwriten;
		ptr += nwriten;
	}
	return (n);
}
