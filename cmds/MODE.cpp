/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:11:38 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 03:22:40 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

// i - invite-only channel flag; 					 	  || MODE #channel +i
// t - topic settable by channel operator only flag;	  || MODE #channel +t
// k - set a channel key (password).				 	  || MODE #channel +k "new-key"
// o - give/take channel operator privileges;			  || MODE #channel +o nick
// l - set the user memberLimit to channel;			  	  || MODE #channel +l 50

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
	if (plusSign)	{
		channel.setKey(key);
		channel.setNeedKey(true);
	}
	else	{
		channel.setKey("");
		channel.setNeedKey(false);
	}
}

void    handleLimitFlag(Channel &channel, bool plusSign, std::string& memberLimit)
{
    if (plusSign)
        channel.setLimitedUsers(std::atoi(memberLimit.c_str()));
    else
        channel.setLimitedUsers(0); // hadi -1 wla 0 ??? 
}

void handleOperatorFlag(Channel &channel, bool plusSign, Client& target)
{
	if (plusSign)
		channel.giveOperator(target);
	else
		channel.removeOperator(target);
}

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

void	Server::handleModeCommand(t_parc &parc, Client& cli)	{
	if (parc.params.size() < 1)
		throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
	if (!Channel::isValidChannelName(parc.params[0]))	{
		throw std::runtime_error("403 * " + parc.params[0] + " :No such channel");
	}
	Channel *channel = getChannelByName(parc.params[0]);
	if (!channel)	{
		throw std::runtime_error("403 * " + parc.params[0] + " :No such channel");
	}
	if (!(*channel).CheckClientExistInChannel(cli))
		throw std::runtime_error("442 " + cli.getNickName() + " " + parc.params[0] + " :You're not on that channel");
	if (parc.params.size() == 1)
		throw std::runtime_error("324 " + cli.getNickName() + " " + parc.params[0] + " " + (*channel).getChannelModes());
	if (!channel->isOperator(cli))
	{
		throw std::runtime_error("482 " + cli.getNickName() + " " + parc.params[0] + " :You're not channel operator");
		return ;
	}
	bool								plusSign = true;
	std::string							firstArg = parc.params[1];
	std::deque<std::string>::iterator	flagArgIt = parc.params.begin() + 2;
	bool								foundAMode = false;
	for (size_t i = 0; i < firstArg.size(); i++)
	{
		if (firstArg[i] == '+')
			plusSign = true;
		else if (firstArg[i] == '-')
			plusSign = false;
		else
		{
			foundAMode = true;
			if (std::string("itkol").find(firstArg[i]) != std::string::npos)
			{
				if (firstArg[i] == 'i')
					handleInviteFlag(*channel, plusSign);
				else if (firstArg[i] == 't')
					handleTopicFlag(*channel, plusSign);
				else if (firstArg[i] == 'k')
				{
					if (flagArgIt == parc.params.end())
					{
						std::string	message = "461 " + cli.getNickName() + " " + parc.cmd + " :Not enough parameters" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					if (!plusSign && *flagArgIt != (*channel).getKey())
					{
						std::string message = "696 " + cli.getNickName() + " " + firstArg[i] + " " + *flagArgIt + " :Invalid MODE flag paramater" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					handleKeyFlag(*channel, plusSign, *flagArgIt);
					flagArgIt++;
				}
				else if (firstArg[i] == 'o')
				{
					if (flagArgIt == parc.params.end())
					{
						std::string	message = "461 " + cli.getNickName() + " " + parc.cmd + " :Not enough parameters" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					Client *target = getClientByNick(*flagArgIt);
					if (!target)
					{
						std::string	message = "461 " + cli.getNickName() + " " + parc.cmd + " :No such Nick/channel" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue ;
					}
					handleOperatorFlag(*channel, plusSign, *target);
					flagArgIt++;
				}
				else if (firstArg[i] == 'l')
				{
					if (flagArgIt == parc.params.end())
					{
						std::string	message = "461 " + cli.getNickName() + " " + parc.cmd + " :Not enough parameters" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					if (isValidNum(*flagArgIt) == false)
					{
						std::string message = "696 " + cli.getNickName() + " " + firstArg[i] + " " + *flagArgIt + " :Invalid MODE flag paramater" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					if (std::atoi((*flagArgIt).c_str()) < 0)
					{
						std::string message = "696 " + cli.getNickName() + " " + firstArg[i] + " " + *flagArgIt + " :Invalid MODE flag paramater" + "\r\n";
						send(cli.getFd(), message.c_str(), message.size(), 0);
						continue;
					}
					handleLimitFlag(*channel, plusSign, *flagArgIt);
					flagArgIt++;
				}
				if (firstArg[i] == 'i' || firstArg[i] == 't')	{
					std::string modestring = (parc.params[0] + " " + (plusSign ? "+" : "-") + firstArg[i]);
					std::string	message = ":" + cli.getNickName() + " MODE " + modestring + "\r\n";
					channel->broadcastMessage(NULL, message);
				}
				else if (firstArg[i] == 'k' || firstArg[i] == 'o' || firstArg[i] == 'l')
				{
					--flagArgIt;
					std::string modestring =  (parc.params[0] + " " + (plusSign ? "+" : "-") + firstArg[i] + " " + *flagArgIt);
					std::string	message = ":" + cli.getNickName() + " MODE " + modestring + "\r\n";
					channel->broadcastMessage(NULL, message);
				}
			}
			else
			{
				std::string	message = "472 " + cli.getNickName() + " " + firstArg[i] + " :is unknown mode char to me" + "\r\n";
				send(cli.getFd(), message.c_str(), message.size(), 0);
				continue;
			}
		}
	}
	if (!foundAMode)
	{
		std::string	message = "461 " + cli.getNickName() + " " + parc.cmd + " :Not enough parameters" + "\r\n";
		send(cli.getFd(), message.c_str(), message.size(), 0);
		return ;
	}
}