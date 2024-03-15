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
		int			channels_joined;
	public:
		Client();
		~Client();

	public:
		void		setFd(int fd);
		void		setNickName(std::string nick);
		void		setRealName(std::string real);
		void		setUserName(std::string user);
    void	increment_channels_joined();
		void 		setRegistrationState(int newState);
		
	public:
		int			getFd( void ) const;
		std::string	getNickName() const;
		std::string	getRealName() const;
  
		std::string	getUserName() const;
		int 		getRegistrationState() const;	
    int getChannelsJoined() const;

class	Channel	{
	private:
		std::string			name;
		std::vector<Client>	users;
		bool				needKey;
		std::string			key;
	public:
		Channel();
		Channel(std::string name) : name(name), needKey(0){}; //name should be transformed to lower (case sensitivity)
		Channel(std::string name, std::string key) : name(name), needKey(1), key(key){}
		~Channel();

		void    setName(std::string name);
		void	addClientToChannel(Client& Cli, size_t i, std::vector<std::string> keys);

		std::string 		getName( void );
		std::vector<Client> get_users();
		int 				getClientsNumber();
		
		int		CheckClientExistInChannel(Client &cli);
	


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
		void    sendToChannel(Client& cli, std::string dest, std::string message);
		Channel		searchChannel(std::string name);
		void 		sendInitialServerReplies(Client &cli);

  
  int	handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli);
  int check_valid_channel_name(std::string channel_name);
std::vector<std::string> split(const std::string &s, const std::string &delim);
  
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