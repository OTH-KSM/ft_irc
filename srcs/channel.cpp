/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:48:12 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 21:26:32 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"


Channel::Channel()  {
    
}

Channel::Channel(std::string name) : name(lower_string(name)), needKey(0)
{
}

Channel::Channel(std::string name, std::string key) :name(lower_string(name)), needKey(1), key(key)
{
}
Channel::~Channel() {
    
}

/*                                        _   
* _ __   _____      __  _ __   __ _ _ __| |_ 
*| '_ \ / _ \ \ /\ / / | '_ \ / _` | '__| __|
*| | | |  __/\ V  V /  | |_) | (_| | |  | |_ 
*|_| |_|\___| \_/\_/   | .__/ \__,_|_|   \__|
*                      |_|                   
*/

/* Getters */

bool	Channel::isValidChannelName(const std::string name)
{
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ',' || name[i] == ' ')
			return false;
	}
	return true;
}

bool    Channel::getInviteOnly() {
    return isInviteOnly;
}

bool   Channel::getTopicRestricted() {
    return topicRestricted;
}

bool	Channel::getHaveTopic()
{
	return(haveTopic);
}
std::string Channel::getTopic() {
    return topic;
}

int     Channel::getLimitedUsers() {
    return limitedUsers;
}

std::string Channel::getKey() {
    return key;
}

std::string Channel::getTopicSetter()
{
	return(topicSetter);
}

std::string Channel::getTimeTopicWasSet()
{
	return(timeTopicWasSet);
}

int 		Channel::getNumberOfUsers()
{
	int i = 0;
	for(std::vector<ChannelMember>::iterator ite = users.begin(); ite != users.end(); ite++)
	{
		i++;
	}
	return (i);
}
/* Setters */

void    Channel::setInviteOnly(bool isInviteOnly) {
    this->isInviteOnly = isInviteOnly;
}

void    Channel::setTopicRestricted(bool isTopicRestricted) {
    this->topicRestricted = isTopicRestricted;
}

void	Channel::setHaveTopic(bool topic_bool)
{
	haveTopic = topic_bool;
}

void    Channel::setTopic(std::string topic) {
    this->topic = topic;
}

bool    Channel::getNeedKey() {
    return needKey;
}

void    Channel::setNeedKey(bool needKey) {
    this->needKey = needKey;
}

void    Channel::setLimitedUsers(int limitedUsers) {
    this->limitedUsers = limitedUsers;
}

void    Channel::setKey(std::string key) {
    this->key = key;
}

void	Channel::setTopicSetter(std::string nick){
	topicSetter = nick;
}

void	Channel::setTimeTopicWasSet(std::string t)
{
	timeTopicWasSet = t;
}

bool	Channel::isOperator(Client &client)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].client.getNickName() == client.getNickName())
			return this->users[i].isOperator;
	}
	return false;
}

void	Channel::clearTopic()
{
	topic.clear();
	haveTopic = 0;
}

void			Channel::kickUser(std::string User_nick)
{
	int i = 0;
	for (std::vector<ChannelMember>::iterator ite = users.begin(); ite != users.end(); ite++)
	{
		if ((*ite).client.getNickName() == User_nick)
		{
			users.erase(users.begin() + i); // gotta check it more
			return ;
		}
		i++;
	}
}

void Channel::giveOperator(Client &cli)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].client.getNickName() == cli.getNickName())
		{
			this->users[i].isOperator = true;
			return ;
		}
	}
}

void Channel::removeOperator(Client &client)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].client.getNickName() == client.getNickName())
		{
			this->users[i].isOperator = false;
			return ;
		}
	}
}

void Channel::addClientToChannel(Client& cli, size_t i, std::vector<std::string> keys, bool op) {
    if(needKey == 0 || (needKey == 1 && keys.size() >= i + 1 && keys[i] == key))
    {
        if(cli.getChannelsJoined() == 10)
            throw std::runtime_error(" :Client cant join more than 10 client");
        ChannelMember member;
        member.client = cli;
        member.isOperator = op;
        this->users.push_back(member);
        cli.increment_channels_joined();
    }
    return;
}

std::vector<ChannelMember> Channel::get_users()
{
    return(users);
}

int     Channel::getClientsNumber(){
    return(users.size());
}
void    Channel::setName(std::string name)   {
    this->name = lower_string(name);
}

std::string Channel::getName( void )    {
    return lower_string(this->name);
}

int  Channel::CheckClientExistInChannel(Client &cli)
{
    for(std::vector<ChannelMember>::iterator ite = users.begin(); ite != users.end(); ite++)
    {
        if((*ite).client.getNickName() == cli.getNickName())   {
            return(1);
        }
    }
    return(0);
}

int  Channel::CheckClientExistInChannel(std::string client_nick)
{
    for(std::vector<ChannelMember>::iterator ite = users.begin(); ite != users.end(); ite++)
    {
        if((*ite).client.getNickName() == client_nick)   {
            return(1);
        }
    }
    return(0);
}

void Channel::broadcastMessage(Client *sender, std::string message)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (sender && this->users[i].client.getNickName() == sender->getNickName())
			continue ;
		send(this->users[i].client.getFd(), message.c_str(), message.length(), 0);
	}
}

void Channel::broadcastMessageServer(std::string message)
{
	for (size_t i = 0; i < this->users.size(); i++)
		send(this->users[i].client.getFd(), message.c_str(), message.length(), 0);
}

void Channel::removeMember(Server &srv, Client client)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (this->users[i].client.getNickName() == client.getNickName())
		{
			this->users.erase(this->users.begin() + i);
			break ;
		}
	}
	if (!this->get_users().size())
		srv.removeChannel(*this);
}

std::string	lower_string(std::string &name)
{
	std::string ret;
	for(std::string::iterator ite = name.begin(); ite != name.end(); ite++)
		ret.push_back(tolower(*ite));
	return(ret);
}

