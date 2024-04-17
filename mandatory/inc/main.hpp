/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42barcelona>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 11:23:40 by eavedill          #+#    #+#             */
/*   Updated: 2024/04/18 00:03:41 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "unp.h"
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>


int Socket(int family, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
int Fork(void);
int Execve(const char *pathname, char *const argv[], char *const envp[]);
int Close(int sockfd);
ssize_t Writen(int filedes, void *buff, size_t nbytes);
ssize_t Readn(int filedes, void *buff, size_t nbytes);
ssize_t Readline(int filedes, void *buff, size_t maxlen);
void str_echo(int sockfd);

ssize_t readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, const void *vptr, size_t n);
ssize_t readline(int fd, void *vptr, size_t maxlen);
