/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/02/26 10:20:57 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/************************ Server ************************/

Server::Server() : name("MainServer") {
    
}

Server::~Server() {
        
}

/************************ Channel ************************/

Channel::Channel(std::string uName) : name(uName)  {
    
}

Channel::~Channel() {
    
}

void    Channel::setName(std::string uName) {
    this->name = uName;
}

void    Channel::setTopic(std::string uTopic)   {
    this->topic = uTopic;
}

std::string    Channel::getName( void )    {
    return this->name;
}

std::string    Channel::getTopic( void )   {
    return this->topic;
}


/************************ Client ************************/


Client::Client() : nickName("default"), userName("default"), fullName("default") {
    
}

Client::~Client() {
    
}

void    Client::setNickName(std::string nick) {
    this->nickName = nick;
}

void    Client::setUserName(std::string user) {
    this->userName = user;
}

void    Client::setFullName(std::string full) {
    this->fullName = full;
}

std::string    Client::getNickName( void ) {
    return this->nickName;
}

std::string    Client::getUserName( void ) {
    return this->userName;
}

std::string    Client::getFullName( void ) {
    return this->fullName;
}


/************************ GENERAL ************************/

void    parc(std::string input) {
    
}