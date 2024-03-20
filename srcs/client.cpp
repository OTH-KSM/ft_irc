/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:50:00 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 09:13:12 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

Client::Client() : registrationState(0) , channels_joined(0)  {
    
}

Client::~Client()   {}

void    Client::setFd(int fd)   {
    this->FD = fd;
}

void    Client::setNickName(std::string nick) {
    nickname = nick;
}

void    Client::setUserName(std::string user) {
    username = user;
}

void    Client::setRealName(std::string real) {
    realname = real;
}

void Client::setRegistrationState(int newState) {
    registrationState = newState;
}

void    Client::increment_channels_joined(){
    channels_joined++;
}

int     Client::getFd( void )   const   {
    return this->FD;
}

std::string Client::getNickName() const {
    return nickname;
}

std::string Client::getUserName() const {
    return username;
}

std::string Client::getRealName() const {
    return realname;
}

int Client::getRegistrationState() const {
    return registrationState;
}

int Client::getChannelsJoined() const {
    return channels_joined;
}

void	Client::addChannelInvitation(const std::string& channel)
{
	this->channelInvitations.push_back(channel);
}