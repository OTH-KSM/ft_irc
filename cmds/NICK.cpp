/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:52:10 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/23 21:49:29 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void	Server::handleNickCommand(t_parc &parc, Client& cli)	{
	int state = cli.getRegistrationState();
	std::string nick;
	if (state != 1 && state != 3) {
		throw std::runtime_error("451 * :You have not registered");
	}
	nick = parc.params.front();
	if (nick.empty())
		throw std::runtime_error("431 * :No nickname given");
	if (nick.length() > 9 || nick.find_first_of(" ,*?!@.#") != std::string::npos)
		throw std::runtime_error("432 * " + nick + " :Erroneus nickname");
	Client *target = getClientByNick(parc.params.front());
	if (target)	{
		throw std::runtime_error("433 * " + nick + "::Nickname is already in use");
	}
	cli.setNickName(nick);
	if (state != 3)
		cli.setRegistrationState(2); // 2 -> user
}