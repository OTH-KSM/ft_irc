/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:21:01 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 21:22:57 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "client.hpp"
#include "channel.hpp"

class File;

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
		std::vector<File>		files;

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
			exit(0);
		}

	public:
		void		handlePassCommand(t_parc &parc, Client& cli);
		void		handleNickCommand(t_parc &parc, Client& cli);
		void		handleUserCommand(t_parc &parc, Client& cli);
		void		handlePrivmsgCommand(t_parc &parc, Client& cli);
		void    	handleJoinCommand(t_parc &parc, Client& cli);
		void    	handleQuitCommand(Server &srv, int i, fd_set &master, bool isForced);
		void		handleModeCommand(t_parc &parc, Client& cli);
		void		handleInviteCommand(t_parc &parc, Client &client);
		int			handleTopicCommand(t_parc &parc, Client &client);
		int			handleKickCommand(t_parc &parc, Client &client);

		void    	handleSendFileCommand(t_parc &parc, Client &cli);
		void    	handleGetFileCommand(t_parc &parc, Client &cli);

		void		Bot(t_parc &parc, Client &cli);

	public:
		void 		removeChannel(Channel channel);
		void    	join_server_response(Client &cli, Channel &channel) ;
		
};


#endif