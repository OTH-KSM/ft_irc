/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/02/29 13:25:25 by okassimi         ###   ########.fr       */
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


Client::Client() {
    
}

Client::~Client() {
    
}

void    Client::setNickName(std::string nick) {
    this->nickName = nick;
}

void    Client::setUserName(std::string user) {
    this->userName = user;
}

std::string    Client::getNickName( void ) {
    return this->nickName;
}

std::string    Client::getUserName( void ) {
    return this->userName;
}


/************************ GENERAL ************************/

// int validateParam(std::string param)    {
//     if (param.empty() || param.length() > 9 || param.find_first_of(" ,*?!@.") != std::string::npos)
//         std::runtime_error(param + ERR_ERRONEUSNICKNAME);  
// }


void    parc(std::string message) {
    t_parc      parc;
    std::string temp;
    std::stringstream cc;

    size_t prefixEnd = message.find(":", 2);
    if (prefixEnd != std::string::npos) {
        std::string prefix = message.substr(prefixEnd + 1, message.length() - 1);
        message.erase(prefixEnd, message.length() - 1);
        cc.str(message);
        while (cc >> temp)  {
            parc.params.push_back(temp);
        }
        parc.params.push_back(prefix);
    }
    cc.str(message);
    while (cc >> temp)  {
        parc.params.push_back(temp);
    }
    if (!parc.params.empty() && !parc.params.front().empty() &&  parc.params.front()[0] == ':')  {
        parc.prefix = parc.params.front();
        parc.params.pop_front();
    }
    if (!parc.params.empty())    {
        parc.cmd = parc.params.front();
        parc.params.pop_front();
    }
    if (parc.params.size() > 15)
        std::runtime_error("Error: More than 15 parameters are not allowed.\n");
    
    // std::deque<std::string>::iterator it;
    // int i = 0;
    //     std::cout << "prefix -> " << parc.prefix << std::endl;
    //     std::cout << "comand -> " << parc.cmd << std::endl;
    // for (it = parc.params.begin(); it != parc.params.end(); it++)   {
    //     std::cout << "param  " << i++ << " -> " << *it << std::endl;
    // }
    
    // if (parc.cmd == "NICK") {
    //     validateParam(parc.params.front())
    // }
    // else if (parc.cmd == "USER")    {
        
    // }
    // adding the user inside the server
    
}
