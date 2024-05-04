/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 22:33:15 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/04 12:58:22 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Our own header. Tabs are set for 4 spaces, not 8 */

#pragma once
/* If anything changes in the following list of #includes, must change acsite.m4 also, for configure's tests. */

#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192
#define SERV_PORT 9877

#define SA struct sockaddr

#define INDEX "./www/index.html"
