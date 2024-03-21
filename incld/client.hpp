/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:19:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:20:47 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"

class Client
{
	private:
		int			FD;
		std::string	nickname;
		std::string	username;
		std::string	realname;
		int			registrationState;
		int			channels_joined;
		std::vector<std::string>	channelInvitations;
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
		std::string	getUserName() const;
		std::string	getRealName() const;
		int 		getRegistrationState() const;	
	public:
    	void		increment_channels_joined();
    	int 		getChannelsJoined() const;
		void		addChannelInvitation(const std::string& channel);
		int			checkClientGotInvitation(const std::string &channel);
};

#endif