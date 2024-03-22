/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:18:19 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 04:22:28 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include "common.hpp"
#include "client.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "bot.hpp"
#include "File.hpp"



void	handleInviteFlag(Channel &channel, bool plusSign);
void	handleTopicFlag(Channel &channel, bool plusSign);
void	handleKeyFlag(Channel &channel, bool plusSign, std::string& key);
bool	isValidNum(const std::string &str);
void	handleLimitFlag(Channel &channel, bool plusSign, std::string& memberLimit);
void 	handleOperatorFlag(Channel &channel, bool plusSign, Client& target);

#endif