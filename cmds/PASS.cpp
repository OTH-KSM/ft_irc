/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:50:56 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 08:51:27 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void	Server::handlePassCommand(t_parc &parc, Client& cli)	{
	int state = cli.getRegistrationState();
	if (state != 1) {
		if (state != 0) {
			if (state == 2)
				throw std::runtime_error("421 * :Unexpected command DO: USER");
			throw std::runtime_error("462 * :You are already connected and cannot handshake again");
		}
		if (parc.params.empty())
			throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
		if (parc.params.front() != this->password)
			throw std::runtime_error("464 * :Incorrect password provided with PASS command.");
		cli.setRegistrationState(1); // 1 -> nick
	}
}