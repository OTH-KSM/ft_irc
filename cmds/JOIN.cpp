/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:05:39 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/23 15:46:09 by okassimi         ###   ########.fr       */
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
    std::cout << "this is the users_list" << users_list << std::endl; // not tested
    newmsg = ":" + Servername + " 353 " + cli.getNickName() + " @ " + channel.getName() + " :" + users_list + "\r\n"; // handle the channel op
    send(client_fd, newmsg.c_str(), newmsg.size(), 0);
    newmsg = ":" + Servername + " 366 " + cli.getNickName() + " " + channel.getName() + " :End of /NAMES list.\r\n"; // handle the channel op
    send(client_fd, newmsg.c_str(), newmsg.size(), 0);
}

int	Server::handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli) {
	std::string message;
    int client_fd = cli.getFd();

    for(size_t i = 0; i < split_channels.size(); i++)
    {
        if(check_valid_channel_name(split_channels[i]))
            continue;
        for (std::vector<Channel>::iterator channel_ite = channels.begin(); channel_ite != channels.end(); channel_ite++) {// gotta check keys  
            if ((*channel_ite).getName() == split_channels[i])
            {
                if((*channel_ite).getLimitedUsers() != -1 && ((*channel_ite).getNumberOfUsers() == (*channel_ite).getLimitedUsers()))
                {
                    message = ":" + Servername + " 473 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+l) - channel is full, try again later\r\n";
                    send(client_fd, message.c_str(), message.size(), 0);
                    return(0);
                }
                if(((*channel_ite).getInviteOnly() == 1 && cli.checkClientGotInvitation((*channel_ite).getName())) || (*channel_ite).getInviteOnly() == 0)
                {
				    if((*channel_ite).getNeedKey() == 0)
				    {
                    	if((*channel_ite).CheckClientExistInChannel(cli) == 0)
                    	{
                        	// std::cout << "new client was added to " << (*channel_ite).getName() << std::endl;
                        	if((*channel_ite).getNumberOfUsers() == 0)
                                (*channel_ite).addClientToChannel(cli, i, split_keys, true);
                            else
                                (*channel_ite).addClientToChannel(cli, i, split_keys, false);
                        	// std::cout << "channels " << channels.size() << " " << "channel_clients in " << (*channel_ite).getName() << " is " << (*channel_ite).getClientsNumber() << std::endl;
                            message = ":" + cli.getNickName() + " JOIN " + (*channel_ite).getName() + "\r\n";
                        	(*channel_ite).broadcastMessage(&cli, message);
                            join_server_response(cli, (*channel_ite));
				    		return 1; //channel exist
                    	}
                    	else
               	    	{
                        	std::cout << "channels " << channels.size() << " " << "channel_clients in " << (*channel_ite).getName() << " is " << (*channel_ite).getClientsNumber() << std::endl;
                        	return 0; // client already exist in channel;
                    	}
				    }
                    else
                    {
                        if((*channel_ite).CheckClientExistInChannel(cli) == 1)
                            return 0;
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
				    		    return 1;
                            }
                            else
                                throw std::runtime_error("475 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+k) - bad key");
                        }
                        else
                        {
                            message = ":" + Servername + " 475 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+k)\r\n";
                            send(client_fd, message.c_str(), message.size(), 0);
                            return(0);
                        }
                    }
                }
                else
                {
                    message = ":" + Servername + " 473 " + cli.getNickName() + " " + (*channel_ite).getName() + " :Cannot join channel (+i)\r\n";
                    send(client_fd, message.c_str(), message.size(), 0);
                    return(0);
                }
            }
        }
        std::cout << "new channel was created " << std::endl;
        if(split_keys.size() >= i + 1)
        {
            Channel new_channel(split_channels[i], split_keys[i]);
            new_channel.addClientToChannel(cli, i, split_keys, true); // this client should become a client operator u gotta handle it.
            channels.push_back(new_channel);
            join_server_response(cli, new_channel);
        }
        else
        {
            Channel new_channel(split_channels[i]);
            new_channel.addClientToChannel(cli, i, split_keys, true); // this client should become a client operator u gotta handle it.
            channels.push_back(new_channel);
            join_server_response(cli, new_channel);
        }
        std::cout << "channels " << channels.size() << " " << "channel_clients in " << channels[i].getName() << " is " << channels[i].getClientsNumber() << std::endl;
    }
    return 0;
}
