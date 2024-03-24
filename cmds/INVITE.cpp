/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:09:20 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 16:46:24 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

// INVITE <nickname> <channel>
//          [0] 	   [1]

void	Server::handleInviteCommand(t_parc &parc, Client &cli)
{
	int state = cli.getRegistrationState();
    if (state != 3)
        throw std::runtime_error("451 * :You have not registered");
	if (parc.params.size() < 2)
	{
		throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
		return ;
	}
	parc.params[1] = lower_string(parc.params[1]);
	Channel *channel = getChannelByName(parc.params[1]);
	if (!channel)
	{
		throw std::runtime_error("403 " + cli.getNickName() + " " + parc.params[1] + " :No such channel");
		return ;
	}
	if (channel && channel->getInviteOnly() && !channel->isOperator(cli))	{
		throw std::runtime_error("482 " + cli.getNickName() + " " + parc.params[1] + " :You're not channel operator");
	}
	if (channel && !channel->CheckClientExistInChannel(cli)){
		throw std::runtime_error("442 " + cli.getNickName() + " " + parc.params[1] + " :You're not on that channel");
	}
	parc.params[0] = lower_string(parc.params[0]);
	Client *target = getClientByNick(parc.params[0]);
	if (!target)
	{
		throw std::runtime_error("401 " + cli.getNickName() + " " + parc.params[0] + " :No such Nick");
		return ;
	}
	if (channel && channel->CheckClientExistInChannel(*target))
	{
		throw std::runtime_error("443 " + cli.getNickName() + " " + parc.params[0] + " " + parc.params[1] + " :is already on channel");
	}
	target->addChannelInvitation(parc.params[1]);
	std::string	message = ":" + cli.getNickName() + " INVITE " + parc.params[0] + " " + parc.params[1] + "\r\n";
	send(target->getFd(), message.c_str(), message.size(), 0);
	
}
