/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:17:10 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 21:26:28 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"
#include "client.hpp"
#include "server.hpp"

class Server;

typedef struct
{
	Client	client;
	bool	isOperator;
}	ChannelMember;

class	Channel	{
	private:
		std::string					name;
		std::vector<ChannelMember>	users;
		bool				needKey;
		std::string			key;
		bool				haveTopic;
		std::string			topic;
		std::string			topicSetter;
		std::string			timeTopicWasSet;
		bool				isInviteOnly;
		bool				topicRestricted;
		int					limitedUsers;
	
	public:
		static bool			isValidChannelName(const std::string name);
		bool			getInviteOnly();
		bool			getTopicRestricted();
		bool			getHaveTopic();
		std::string		getTopic();
		int				getLimitedUsers();
		std::string		getKey();
		bool			getNeedKey();
		std::string 	getChannelModes(void);
		std::string		getTopicSetter();
		std::string		getTimeTopicWasSet();
		int				getNumberOfUsers();

		bool			isOperator(Client &client);

		

		void			setInviteOnly(bool isInviteOnly);
		void			setTopicRestricted(bool isTopicRestricted);
		void			setHaveTopic(bool topic_bool);
		void			setTopic(std::string topic);
		void			setKey(std::string key);
		void			setNeedKey(bool needKey);
		void			setTopicSetter(std::string nick);
		void			setTimeTopicWasSet(std::string t);

		void			clearTopic();
		void			kickUser(std::string User_nick);

		void 			giveOperator(Client &client);
		void 			removeOperator(Client &client);
		void 			setLimitedUsers(int limitedUsers);
		

	public:
		Channel();
		Channel(std::string name); //name should be transformed to lower (case sensitivity)
		Channel(std::string name, std::string key);
		~Channel();

		void    setName(std::string name);
		void	addClientToChannel(Client& Cli, size_t i, std::vector<std::string> keys, bool op);

		std::string 		getName( void );
		std::vector<ChannelMember> get_users();
		int 				getClientsNumber();
		
		int		CheckClientExistInChannel(Client &cli);
		int  	CheckClientExistInChannel(std::string client_nick);

		/* the new part */
		void	broadcastMessage(Client *sender, std::string message);
		void	broadcastMessageServer(std::string message);
		void	removeMember(Server &srv, Client client);
		
};

std::string		lower_string(std::string &name);

#endif