/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 22:33:15 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/17 23:42:38 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Our own header. Tabs are set for 4 spaces, not 8 */

#pragma once
/* If anything changes in the following list of #includes, must change acsite.m4 also, for configure's tests. */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>

#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192
#define SERV_PORT 9877

#define SA struct sockaddr

#define INDEX "./www/index.html"
