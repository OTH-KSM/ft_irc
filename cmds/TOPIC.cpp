/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 21:18:19 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

int		Server::handleTopicCommand(t_parc &parc, Client &cli)
{
    Channel *channel_ptr;
    std::string newmsg;
    int state = cli.getRegistrationState();
    int client_fd = cli.getFd();

    if (state != 3)
        throw std::runtime_error("451 * :You have not registered");
    if (parc.params.size() < 1)
        throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
    if(parc.params.size() >= 1)
    {
        parc.params[0] = lower_string(parc.params[0]);
        channel_ptr = getChannelByName(parc.params[0]);
        if(!channel_ptr)
        {
            newmsg = ":" + Servername + " 403 " + cli.getNickName() + " " + parc.params[0] + " :No such channel\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0);
        }
        if(channel_ptr && channel_ptr->CheckClientExistInChannel(cli) == 0)
        {
            newmsg = ":" + Servername + " 442 " + cli.getNickName() + " " + channel_ptr->getName() + " :You're not on that channel\r\n";
            send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            return(0);
        }
        else if(channel_ptr && parc.params.size() == 1)
        {
            if(channel_ptr->getHaveTopic() == 1)
            {
                newmsg = ":" + Servername + " 332 " + cli.getNickName() + " " + channel_ptr->getName() + " :" + channel_ptr->getTopic() + "\r\n";
                send(client_fd, newmsg.c_str(), newmsg.size(), 0);
                newmsg = ":" + Servername + " 333 " + cli.getNickName() + " " + channel_ptr->getName() + " " + channel_ptr->getTopicSetter() + " " + channel_ptr->getTimeTopicWasSet() + "\r\n";
                send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            }
            else
            {
                newmsg = ":" + Servername + " 331 " + cli.getNickName() + " " + channel_ptr->getName() + " :No topic is set\r\n";
                send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            }

        }
        else if(channel_ptr && parc.params.size() >= 2 && parc.params[1] == ":")
        {
            if((channel_ptr->getTopicRestricted() == 1 && channel_ptr->isOperator(cli) == 1) ||  channel_ptr->getTopicRestricted() == 0)
            {
                newmsg = ":" + Servername + " TOPIC " + channel_ptr->getName() + ":\r\n";
                channel_ptr->clearTopic();
                channel_ptr->broadcastMessageServer(newmsg);
            }
            else
            {
                newmsg = ":" + Servername + " 482 " + cli.getNickName() + " " + channel_ptr->getName() + " :You're not channel operator\r\n";
                send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            }
        }
        else if(channel_ptr && parc.params.size() >= 2 && parc.params[1].size() > 1)
        {
            if((channel_ptr->getTopicRestricted() == 1 && channel_ptr->isOperator(cli) == 1) ||  channel_ptr->getTopicRestricted() == 0){
                newmsg = ":" + cli.getNickName() + " TOPIC " + channel_ptr->getName() + " " + parc.params[1] + "\r\n";
                std::time_t t = std::time(0);
                channel_ptr->setTopic(parc.params[1]);
                channel_ptr->setHaveTopic(1);
                channel_ptr->setTopicSetter(cli.getNickName());
                channel_ptr->setTimeTopicWasSet(std::ctime(&t));
                channel_ptr->broadcastMessageServer(newmsg);
            }
            else
            {
                newmsg = ":" + Servername + " 332 " + cli.getNickName() + " " + channel_ptr->getName() + " :You're not channel operator\r\n";
                send(client_fd, newmsg.c_str(), newmsg.size(), 0);
            }
        }
        
    }
    return(1);
}