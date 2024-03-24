/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:55:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 03:26:32 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"


void Server::handleQuitCommand(Server &srv, int i, fd_set &master, bool isForced) {
	std::string quitMessage;
	if (isForced == false)	{
		int state = clientMap[i].getRegistrationState();
		if (state!= 3)	{
			close(clientMap[i].getFd());
			return ;
		}
	}
    std::vector<Channel> channels = getChannels();

	
    for (size_t j = 0; j < channels.size(); j++)
    {
        if (channels[j].CheckClientExistInChannel(clientMap[i]))
        {
            channels[j].broadcastMessage(&clientMap[i], ":" + clientMap[i].getNickName() + " QUIT :leaving...\r\n");
            channels[j].removeMember(srv, clientMap[i]);
        }
    }	
	int fd = i;
	
    FD_CLR(i, &master);
	close(fd);
    clientMap.erase(fd);
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