/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:21:01 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:22:24 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "client.hpp"
#include "channel.hpp"

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
		Client*					getClientByNick(std::string nick);
		Client&					getClientByFd(int fd);
		std::vector<Channel> 	getChannels();
		Channel*				getChannelByName(std::string name);

		
		void		printClients( void );
		int			isClientExist(std::string nick, int fd);
		void		sendOneToOne(Client& cli, std::string dest, std::string message);
		void    	sendToChannel(Client& cli, std::string dest, std::string message);
		Channel		searchChannel(std::string name);
		void 		sendInitialServerReplies(Client &cli);

  
		int							handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli);
		int 						check_valid_channel_name(std::string channel_name);
		std::vector<std::string>	split(const std::string &s, const std::string &delim);
  
		static void SignalHandler(int signum) {
			(void)signum;
			std::cout << std::endl << "Signal Received!" << std::endl;
			Signal = true;
		}

	public:
		void		handlePassCommand(t_parc &parc, Client& cli);
		void		handleNickCommand(t_parc &parc, Client& cli);
		void		handleUserCommand(t_parc &parc, Client& cli);
		void		handlePrivmsgCommand(t_parc &parc, Client& cli);
		void    	handleJoinCommand(t_parc &parc, Client& cli);
		void    	handleQuitCommand(Server &srv, t_parc &parc, Client& cli);
		void		handleModeCommand(t_parc &parc, Client& cli);
		void		handleInviteCommand(t_parc &parc, Client &client);
		int			handleTopicCommand(t_parc &parc, Client &client);

	public:
		void 	removeChannel(Channel channel);
		void	printChannelsAndClients();
		void    join_server_response(Client &cli, Channel &channel) ;
		
};


#endif