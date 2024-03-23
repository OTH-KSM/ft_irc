/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:55:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/23 15:04:13 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"


void Server::handleQuitCommand(Server &srv, t_parc &parc, Client& cli, fd_set &master) {
	int state = cli.getRegistrationState();
	if (state!= 3)	{
		close(cli.getFd());
		return ;
	}
    std::vector<Channel> channels = getChannels();
    std::string quitMessage = parc.params.size() > 0 ? parc.params[0] : "No reason given";    
    std::string message = ":" + cli.getNickName() + " QUIT :Client Quit\r\n";

    for (size_t j = 0; j < channels.size(); j++)
    {
        if (channels[j].CheckClientExistInChannel(cli))
        {
            channels[j].broadcastMessage(&cli, ":" + cli.getNickName() + " QUIT :" + quitMessage + "\r\n");
            channels[j].removeMember(srv, cli);
        }
    }
	int fd = cli.getFd();

    send(fd, message.c_str(), message.size(), 0);
    FD_CLR(cli.getFd(), &master);
    deleteClient(cli);

	close(fd);
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