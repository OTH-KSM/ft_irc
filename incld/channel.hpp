/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:17:10 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:40:18 by okassimi         ###   ########.fr       */
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
		int					limitedUsers; // -1 if no limitUsers
	
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

		bool		isOperator(Client &client);



		void			setInviteOnly(bool isInviteOnly);
		void			setTopicRestricted(bool isTopicRestricted);
		void			setHaveTopic(bool topic_bool);
		void			setTopic(std::string topic);
		void			setKey(std::string key);
		void			setNeedKey(bool needKey);
		void			setTopicSetter(std::string nick);
		void			setTimeTopicWasSet(std::string t);

		void			clearTopic();
		int				checkValidTopic(std::string topic);

		void 			giveOperator(Client &client);
		void 			removeOperator(Client &client);
		void 			setLimitedUsers(int limitedUsers);
		

		void 		listUsers() const;

	public:
		Channel();
		Channel(std::string name) : name(name), needKey(0){}; //name should be transformed to lower (case sensitivity)
		Channel(std::string name, std::string key) : name(name), needKey(1), key(key){}
		~Channel();

		void    setName(std::string name);
		void	addClientToChannel(Client& Cli, size_t i, std::vector<std::string> keys, bool op);

		std::string 		getName( void );
		std::vector<ChannelMember> get_users();
		int 				getClientsNumber();
		
		int		CheckClientExistInChannel(Client &cli);

		/* the new part */
		void	broadcastMessage(Client *sender, std::string message);
		void	broadcastMessageServer(std::string message);
		void	removeMember(Server &srv, Client client);
		
};

#endif