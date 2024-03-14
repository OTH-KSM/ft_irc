/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:33:51 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/14 21:27:51 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <exception>
#include <sstream>
#include <deque>
#include <utility>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <string>
#include <csignal>


typedef struct message
{
	std::string prefix;
	std::string cmd;
	std::deque<std::string> params;
} t_parc;

class Client
{
	private:
		int			FD;
		std::string	nickname;
		std::string	username;
		std::string	realname;
		int			registrationState;
	public:
		Client();
		~Client();

	public:
		void		setFd(int fd);
		void		setNickName(std::string nick);
		void		setRealName(std::string real);
		void		setUserName(std::string user);
		void 		setRegistrationState(int newState);
		
	public:
		int			getFd( void ) const;
		std::string	getNickName() const;
		std::string	getRealName() const;
		std::string	getUserName() const;
		int 		getRegistrationState() const;	
};

class	Channel	{
	private:
		std::string	name;
		
		std::vector<Client>	users;
	public:
		Channel();
		~Channel();

		void    setName(std::string name);
		void	addClientToChannel(Client& Cli);

		std::string getName( void );
	
		void    broadcast(std::string   message);
};



class Server	{
	private:
		std::string	Servername;
		std::string	Version;
		std::string	CreateDate;
		std::string	ChannelModes;
		int			port;
		std::string	password;
		int			SersocketFD;

		std::map<int, Client>	clientMap;
		std::vector<Channel>	channels;

	public:
		static bool		Signal;
		Server(int port, std::string pass);
		~Server();

		void		init(void);
		void		parc(std::string message, Client& cli);

	public:
		int			getFd( void );
		std::string getServerName( void );

		Client&		getClientByNick(std::string nick);
		Client&		getClientByFd(int fd);

		
		void		printClients( void );
		int			isClientExist(std::string nick, int fd);
		void		sendOneToOne(Client& cli, std::string dest, std::string message);
		void    	sendToChannel(std::string dest, std::string message);
		Channel		searchChannel(std::string name);
		void 		sendInitialServerReplies(Client &cli);

		static void SignalHandler(int signum) {
			(void)signum;
			std::cout << std::endl << "Signal Received!" << std::endl;
			Signal = true;
		}

	public:
		void		handlePassCommand(t_parc &parc, Client& cli);
		void		handleNickCommand(t_parc &parc, Client& cli);
		void		handleUserCommand(t_parc &parc, Client& cli);
		void		handleWhoisCommand(t_parc &parc, Client& cli);
		void		handlePrivmsgCommand(t_parc &parc, Client& cli);
};



#endif