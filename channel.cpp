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

void Channel::addClientToChannel(Client& Cli) {
    this->users.push_back(Cli);
}

void    Channel::setName(std::string name)   {
    this->name = name;
}

std::string Channel::getName( void )    {
    return this->name;
}

void Channel::broadcast(std::string message) {
    for (std::vector<Client>::iterator it = users.begin(); it != users.end(); it++) {
        send((*it).getFd(), message.c_str(), message.size(), 0);
    }
}




