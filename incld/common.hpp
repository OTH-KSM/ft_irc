/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:29:23 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 04:12:22 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <exception>
#include <sstream>
#include <deque>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <string>
#include <csignal>
#include <fstream>

typedef struct message
{
	std::string prefix;
	std::string cmd;
	std::deque<std::string> params;
} t_parc;

#endif