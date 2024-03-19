/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 07:00:09 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/18 07:37:06 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

void handleInviteFlag(Channel &channel, bool plusSign)
{
	if (plusSign)
		channel.setInviteOnly(true);
	else
		channel.setInviteOnly(false);
}

void handleTopicFlag(Channel &channel, bool plusSign)
{
	if (plusSign)
		channel.setTopicRestricted(true);
	else
		channel.setTopicRestricted(false);
}

void handleKeyFlag(Channel &channel, bool plusSign, std::string& key)
{
	if (plusSign)
		channel.setKey(key);
	else
		channel.setKey("");
}

void    handleLimitFlag(Channel &channel, bool plusSign, std::string& memberLimit)
{
    if (plusSign)
        channel.setLimitedUsers(std::atoi(memberLimit.c_str()));
    else
        channel.setLimitedUsers(0);
}

// void handleOperatorFlag(Channel &channel, bool plusSign, Client& target)
// {
// 	if (plusSign)
// 		channel.giveOperator(&target);
// 	else
// 		channel.removeOperator(&target);
// }

// void handleLimitFlag(Channel &channel, bool plusSign, std::string& memberLimit)
// {
// 	if (plusSign)
// 		channel.setmemberLimit(std::atoi(memberLimit.c_str()));
// 	else
// 		channel.setmemberLimit(0);
// }

bool	isValidNum(const std::string &str)
{
	if (str[0] == '-' || str[0] == '+')
		return str.find_first_not_of("0123456789", 1) == std::string::npos;
	else
		return str.find_first_not_of("0123456789") == std::string::npos;
}

std::string Channel::getChannelModes(void)
{
	std::string channelModes;
	std::string channelFlagModes;
	std::string channelArgModes;

	if (this->getInviteOnly())
		channelFlagModes.append("i");
	if (this->getTopicRestricted())
		channelFlagModes.append("t");
	if (this->getLimitedUsers() != -1)
	{
		std::stringstream ss;
		ss << this->getLimitedUsers();
		channelFlagModes.append("l");
		channelArgModes.append(ss.str());
	}
	if (!channelFlagModes.empty())
	{
		channelModes.append(" +");
		channelModes.append(channelFlagModes);
	}
	else
		channelModes.append(" no modes is set");
	channelModes.append(" ");
	channelModes.append(channelArgModes);
	return (channelModes);
}


