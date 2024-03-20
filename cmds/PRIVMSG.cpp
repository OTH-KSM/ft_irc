/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:08:19 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:42:28 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void    Server::sendOneToOne(Client& cli, std::string dest, std::string message)    {
	int destFd = isClientExist(dest, cli.getFd());
	if (destFd == -1)
		throw std::runtime_error(dest + " :No such Nickname\n");
	std::string newmsg = ":" + cli.getNickName() + " PRIVMSG " + dest + " : " + message + "\r\n";
	send(destFd, newmsg.c_str(), newmsg.size(), 0);
}

void    Server::sendToChannel(Client &cli, std::string dest, std::string message) {
    for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if((*it).getName() == dest)
        {
            std::vector<ChannelMember> users = (*it).get_users();
            for(std::vector<ChannelMember>::iterator ite = users.begin(); ite != users.end(); ite++)
            {
				if ((*ite).client.getNickName() == cli.getNickName())
					continue;
                int destFd = (*ite).client.getFd();
                if (destFd == -1)
                    throw std::runtime_error(dest + " :No such nick/channel\n");
					std::cout << (*it).getName() << std::endl;
                std::string newmsg = ":" + cli.getNickName() + " PRIVMSG " + (*it).getName() + " :" + message + "\r\n";
                send(destFd, newmsg.c_str(), newmsg.size(), 0);
            }
        }
    }
}

void	Server::handlePrivmsgCommand(t_parc &parc, Client& cli)	{
    int state = cli.getRegistrationState();
    if (state != 3)
        throw std::runtime_error("451 * :You have not registered");
    if (parc.params.size() < 2)
        throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
    if (parc.params[0][0] == '#' || parc.params[0][0] == '&')
        sendToChannel(cli, parc.params[0], parc.params[1]);
    else
        sendOneToOne(cli, parc.params[0], parc.params[1]);
}

