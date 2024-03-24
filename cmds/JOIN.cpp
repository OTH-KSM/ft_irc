/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:05:39 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 21:19:09 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void    Server::handleJoinCommand(t_parc &parc, Client& cli)    {
    int state = cli.getRegistrationState();
    if (state != 3)
        throw std::runtime_error("451 * :You have not registered");
    if (parc.params.size() < 1)
        throw std::runtime_error(parc.cmd + " :Not enough parameters");
    std::vector<std::string> split_channels;
    std::vector<std::string> split_keys;
    parc.params[0] = lower_string(parc.params[0]);
    if (parc.params.size() >= 1)
        split_channels = split(parc.params[0], ",");
    if(parc.params.size() >= 2)
        split_keys = split(parc.params[1], ",");
    handleChannel(split_channels, split_keys, cli);
}


void    Server::join_server_response(Client &cli, Channel &channel) 
{
    int client_fd = cli.getFd();
    std::string newmsg = ":" + cli.getNickName() + " JOIN " + channel.getName() + "\r\n";
    send(client_fd, newmsg.c_str(), newmsg.size(), 0);
    if(!channel.getTopic().empty())
    {
        newmsg = ":" + Servername + " 332 " + cli.getNickName() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n";
        send(client_fd, newmsg.c_str(), newmsg.size(), 0);
    }
    std::vector<ChannelMember> channel_users = channel.get_users(); 
    std::string users_list;
    for(std::vector<ChannelMember>::iterator ite = channel_users.begin(); ite != channel_users.end(); ite++)
    {
		if ((*ite).isOperator)
            users_list += "@" + (*ite).client.getNickName();
        else
            users_list += (*ite).client.getNickName();
        if((ite + 1) != channel_users.end())
            users_list += " ";
    }
    newmsg = ":" + Servername + " 353 " + cli.getNickName() + " @ " + channel.getName() + " :" + users_list + "\r\n";
    send(client_fd, newmsg.c_str(), newmsg.size(), 0);
    newmsg = ":" + Servername + " 366 " + cli.getNickName() + " " + channel.getName() + " :End of /NAMES list.\r\n";
    send(client_fd, newmsg.c_str(), newmsg.size(), 0);
}

int	Server::handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli) {
	std::string message;
    int continue_flag = 0;
    int client_fd = cli.getFd();

    for(size_t i = 0; i < split_channels.size(); i++)
    {
        if(check_valid_channel_name(lower_string(split_channels[i])))
        {
            continue_flag = 1;
            break;
        }
        for (std::vector<Channel>::iterator channel_ite = channels.begin(); channel_ite != channels.end(); channel_ite++) {
            if ((*channel_ite).getName() == lower_string(split_channels[i]))
            {
                if((*channel_ite).getLimitedUsers() != -1 && ((*channel_ite).getNumberOfUsers() == (*channel_ite).getLimitedUsers()))
                {
                    message = ":" + Servername + " 473 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+l) - channel is full, try again later\r\n";
                    send(client_fd, message.c_str(), message.size(), 0);
                    continue_flag = 1;
                    break;
                }
                if(((*channel_ite).getInviteOnly() == 1 && cli.checkClientGotInvitation((*channel_ite).getName())) || (*channel_ite).getInviteOnly() == 0)
                {
				    if((*channel_ite).getNeedKey() == 0)
				    {
                    	if((*channel_ite).CheckClientExistInChannel(cli) == 0)
                    	{
                        	if((*channel_ite).getNumberOfUsers() == 0)
                                (*channel_ite).addClientToChannel(cli, i, split_keys, true);
                            else
                                (*channel_ite).addClientToChannel(cli, i, split_keys, false);
                            message = ":" + cli.getNickName() + " JOIN " + (*channel_ite).getName() + "\r\n";
                        	(*channel_ite).broadcastMessage(&cli, message);
                            join_server_response(cli, (*channel_ite));
				    		continue_flag = 1;
                            break;
                    	}
                    	else
               	    	{
                        	continue_flag = 1;
                    	}
				    }
                    else
                    {
                        if((*channel_ite).CheckClientExistInChannel(cli) == 1)
                        {
                            continue_flag = 1;
                            break;
                        }
                        else if((split_keys.size() >= i + 1))
                        {
                            if(split_keys[i] == (*channel_ite).getKey())
                            {
                                if((*channel_ite).getNumberOfUsers() == 0)
                                    (*channel_ite).addClientToChannel(cli, i, split_keys, true);
                                else
                                    (*channel_ite).addClientToChannel(cli, i, split_keys, false);
                                message = ":" + cli.getNickName() + " JOIN " + (*channel_ite).getName() + "\r\n";
                        	    (*channel_ite).broadcastMessage(&cli, message);
                        	    join_server_response(cli, (*channel_ite));
				    		    continue_flag = 1;
                                break;
                            }
                            else
                                throw std::runtime_error("475 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+k) - bad key");
                        }
                        else
                        {
                            message = ":" + Servername + " 475 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+k)\r\n";
                            send(client_fd, message.c_str(), message.size(), 0);
                            continue_flag = 1;
                            break;
                        }
                    }
                }
                else
                {
                    message = ":" + Servername + " 473 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+i)\r\n";
                    send(client_fd, message.c_str(), message.size(), 0);
                    continue_flag = 1;
                    break;
                }
            }
        }
        if(continue_flag)
        {
            continue_flag = 0;
            continue;
        }
        if(split_keys.size() >= i + 1)
        {
            Channel new_channel(split_channels[i], split_keys[i]);
            new_channel.addClientToChannel(cli, i, split_keys, true);
            channels.push_back(new_channel);
            join_server_response(cli, new_channel);
        }
        else
        {
            Channel new_channel(split_channels[i]);
            new_channel.addClientToChannel(cli, i, split_keys, true);
            channels.push_back(new_channel);
            join_server_response(cli, new_channel);
        }
    }
    return 0;
}
