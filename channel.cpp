/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:48:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/10 01:33:50 by okassimi         ###   ########.fr       */
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
        if((*ite).getNickname() == cli.getNickname())
            return(1);
    }
    return(0);
}
void Channel::broadcast(std::string message) {
    for (std::vector<Client>::iterator it = users.begin(); it != users.end(); it++) {
        send((*it).getFd(), message.c_str(), message.size(), 0);
    }
}




