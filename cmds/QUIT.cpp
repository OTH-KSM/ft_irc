/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:55:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:55:36 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"


void    Server::handleQuitCommand(Server &srv, t_parc &parc, Client& cli)    {
	std::vector<Channel>	channels = getChannels();
	std::string				quitMessage = parc.params.size() > 0 ? parc.params[0] : "No reason given";
	for (size_t j = 0; j < channels.size(); j++)
	{
		if (channels[j].CheckClientExistInChannel(cli))
		{
			channels[j].broadcastMessage(&cli, ":" + cli.getNickName() + " QUIT :" + quitMessage + "\r\n");
			channels[j].removeMember(srv, cli);
		}
	}
	close(cli.getFd());
}

void Server::removeChannel(Channel channel)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].getName() == channel.getName())
		{
			this->channels.erase(this->channels.begin() + i);
			return;
		}
	}
}