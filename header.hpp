/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:33:51 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/10 11:12:08 by okassimi         ###   ########.fr       */
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


		void	setFd(int fd);
		void	setNickName(std::string nick);
		void	setRealName(std::string real);
		void	setUsername(std::string user);
		void setRegistrationState(int newState);
		

		int		getFd( void ) const;
		std::string	getNickname() const;
		std::string	getRealName() const;
		std::string	getUsername() const;
		int getRegistrationState() const;
		
		
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
	int			port;
	std::string	password;
	int			SersocketFD;

	std::map<int, Client>	clientMap;
	std::vector<Channel>	channels;

public:
	Server(int port, std::string pass);
	~Server();

	void init(void);
	
	int	getFd( void );
	
	void	printClients( void );
	void	sendOneToOne(std::string dest, std::string message);
	void    sendToChannel(std::string dest, std::string message);
	int		getClientNick(std::string nick);
	void 	closeClientsFd();

	
	void parc(std::string message, Client& cli);

	Channel	searchChannel(std::string name);
};

typedef struct message
{
	std::string prefix;
	std::string cmd;
	std::deque<std::string> params;
} t_parc;



#endif