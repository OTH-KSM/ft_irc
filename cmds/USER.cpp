/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:52:55 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:53:04 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void	Server::handleUserCommand(t_parc &parc, Client& cli)	{
	int state = cli.getRegistrationState();
	if (state != 2) {
		if (state == 0 || state == 1)
			throw std::runtime_error("451 * :You have not registered");
		throw std::runtime_error("462 * :You are already connected and cannot handshake again");
	}
	if (parc.params.size() < 4)
		throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
	cli.setUserName(parc.params[0]);
	cli.setRealName(parc.params[3]);
	cli.setRegistrationState(3);
	sendInitialServerReplies(cli);
}