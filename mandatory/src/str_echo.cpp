/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_echo.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:38:35 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/16 23:23:04 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.hpp"

void str_echo (int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE];

	again:
	while ( n = Readn(sockfd, buf, MAXLINE) > 0)
		Writen(sockfd, buf, n);
	if (n < 0 && errno == EINTR)
		goto again;
	if (n < 0)
		err_sys("str_Echo:read error");
}