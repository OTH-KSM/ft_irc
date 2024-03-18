/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:48:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/17 07:03:02 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

Channel::Channel()  {
    
}

Channel::~Channel() {
    
}

void Channel::addClientToChannel(Client& Cli, size_t i, std::vector<std::string> keys) {
    if(needKey == 0 || (needKey == 1 && keys.size() >= i + 1 && keys[i] == key)) //check password
    {
        if(Cli.getChannelsJoined() == 10)
            throw std::runtime_error(" :Client cant join more than 10 client");
        this->users.push_back(Cli);
        Cli.increment_channels_joined();// added this cause max channels is 10 for each client
    }
    return;
}

std::vector<Client> Channel::get_users()
{
    return(users);
}

int     Channel::getClientsNumber(){
    return(users.size());
}
void    Channel::setName(std::string name)   {
    this->name = name;
}

std::string Channel::getName( void )    {
    return this->name;
}

int  Channel::CheckClientExistInChannel(Client &cli)
{
    for(std::vector<Client>::iterator ite = users.begin(); ite != users.end(); ite++)
    {
        if((*ite).getNickName() == cli.getNickName())
            return(1);
    }
    return(0);
}
void Channel::broadcastMessage(Client sender, std::string message)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].getNickName() == sender.getNickName())
			continue ;
		send(this->users[i].getFd(), message.c_str(), message.length(), 0);
	}
}

void Channel::removeMember(Server &srv, Client client)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].getNickName() == client.getNickName())
		{
			this->users.erase(this->users.begin() + i);
			break ;
		}
	}
	if (!this->get_users().size())
		srv.removeChannel(*this);
}



