/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 00:42:00 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

int		Server::handleKickCommand(t_parc &parc, Client &cli)
{
    Channel *channel_ptr;
    std::string newmsg;
    int state = cli.getRegistrationState();
    int client_fd = cli.getFd();

    if (state != 3)
        throw std::runtime_error("451 * :You have not registered");
    if (parc.params.size() < 2)
        throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
    else
    {
        channel_ptr = getChannelByName(parc.params[0]);
        if(!channel_ptr)
        {
            newmsg = ":" + Servername + " 403 " + cli.getNickName() + " " + parc.params[0] + " :No such channel\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0); //failure
        }
        else if(channel_ptr && channel_ptr->CheckClientExistInChannel(cli) == 0)
        {
            newmsg = ":" + Servername + " 442 " + cli.getNickName() + " " + channel_ptr->getName() + " :You're not on that channel\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0); //failure
        }
        else if(channel_ptr && channel_ptr->CheckClientExistInChannel(parc.params[1]) == 0)
        {
            newmsg = ":" + Servername + " 441 " + cli.getNickName() + " " + parc.params[1] + " " + channel_ptr->getName() + " :They aren't on that channel\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0); //failure
        }
        else if(channel_ptr && channel_ptr->isOperator(cli) == false)
        {
            newmsg = ":" + Servername + " 482 " + cli.getNickName() + " " +  channel_ptr->getName() + " :You're not channel operator\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0); //failure
        }
        else
        {
            channel_ptr->kickUser(parc.params[1]);
            newmsg = ":" + cli.getNickName() + " KICK " + channel_ptr->getName() + " " + parc.params[1] + "\r\n";
	        channel_ptr->broadcastMessageServer(newmsg);
        }
    }
    return(1);
}