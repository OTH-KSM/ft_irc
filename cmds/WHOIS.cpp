/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOIS.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 09:08:14 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 09:08:40 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

// void	Server::handleWhoisCommand(t_parc &parc, Client& cli)	{
// 		if (parc.params[0].empty())
// 			throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
// 		Client *targetClient = getClientByNick(parc.params[0]);

// 		std::string nick = targetClient.getNickName(); 
// 		std::string user = targetClient.getUserName(); 
// 		// std::string host = targetClient.getHostname();
// 		std::string realName = targetClient.getRealName(); 
// 		std::string serverName = this->Servername; 
// 		std::string serverInfo = "The best IRC server Ever";

// 		std::string message = ":" + this->Servername + " 311 " + cli.getNickName() + " " + nick + " " + user + " " + "10.11.1.11" + " * :" + realName + "\r\n";
// 		send(cli.getFd(), message.c_str(), message.size(), 0);

// 		message = ":" + this->Servername + " 312 " + cli.getNickName() + " " + nick + " " + serverName + " :" + serverInfo + "\r\n";
// 		send(cli.getFd(), message.c_str(), message.size(), 0);

// 		message = ":" + this->Servername + " 318 " + cli.getNickName() + " " + nick + " :End of WHOIS list\r\n";
// 		send(cli.getFd(), message.c_str(), message.size(), 0);
// }
