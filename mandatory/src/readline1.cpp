/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline1.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 23:26:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/18 00:05:13 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"



ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	size_t n, rc;
	char c, *ptr;

	ptr = (char *) vptr;
	for (n = 1; n < maxlen; n++)
	{
		again:
			if ((rc = read(fd, &c, 1)) == 1)
			{
				*ptr++ = c;
				if (c == '\n')
					break;
			}
			else if (rc == 0)
			{
				*ptr = 0;
				return (n - 1);
			}
			else
			{
				if (errno == EINTR)
					goto again;
				return (-1);
			}
	}
	*ptr = 0;
	return (n);
}
