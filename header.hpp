/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:33:51 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/12 03:50:06 by okassimi         ###   ########.fr       */
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


		void	setFd(int fd);
		void	setNickName(std::string nick);
		void	setRealName(std::string real);
		void	setUsername(std::string user);
		void	increment_channels_joined();
		void setRegistrationState(int newState);
		

		int		getFd( void ) const;
		std::string	getNickname() const;
		std::string	getRealName() const;
		std::string	getUsername() const;
		int getRegistrationState() const;
		int getChannelsJoined() const;
		
		
};

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
		void    broadcast(std::string   message);
};



class Server	{
private:
	std::string	Servername;
	int			port;
	std::string	password;
	int			SersocketFD;

	std::map<int, Client>	clientMap;
	std::vector<Channel>	channels;

public:
	static bool		Signal;
	Server(int port, std::string pass);
	~Server();

	void init(void);
	
	int	getFd( void );
	
	std::string getServerName( void );

	void	printClients( void );
	void	sendOneToOne(Client& cli, std::string dest, std::string message);
	void    sendToChannel(Client& cli, std::string dest, std::string message);
	int		getClientNick(std::string nick, int fd);
	void 	closeClientsFd();

	static void SignalHandler(int signum) {
        (void)signum;
        std::cout << std::endl << "Signal Received!" << std::endl;
        Signal = true;
    }
	void parc(std::string message, Client& cli);

	int	handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli);
};

int check_valid_channel_name(std::string channel_name);
std::vector<std::string> split(const std::string &s, const std::string &delim);
typedef struct message
{
	std::string prefix;
	std::string cmd;
	std::deque<std::string> params;
} t_parc;



#endif