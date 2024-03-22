/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandling.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 03:24:53 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 11:04:33 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

void Server::handleSendFileCommand(t_parc &parc, Client &cli) {
    if (cli.getRegistrationState() != 3)
        throw std::runtime_error("451 * :You have not registred");
    if (parc.params.size() != 2)
        throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
    Client *target = getClientByNick(parc.params[0]);
    if (!target)
        throw std::runtime_error("461 " + cli.getNickName() + " " + parc.params[0] + " :No such Nick");
    std::fstream file(parc.params[1], std::fstream::in);
    if (!file.is_open())
        throw std::runtime_error("0 * :Invalid file path");
    int srch = parc.params[1].find_last_of('/');
    std::string filename = parc.params[1].substr(srch + 1);
    File newFile(filename, parc.params[1], cli.getNickName(), parc.params[0]);
    files.push_back(newFile);
    sendOneToOne(cli, parc.params[0], cli.getNickName() + " wants to send you a file called " + filename);
}

void    Server::handleGetFileCommand(t_parc &parc, Client &cli) {
    if (cli.getRegistrationState() != 3)
        throw std::runtime_error("451 * :You have not registred");
    if (parc.params.size() != 2)
        throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
    File file;
    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].getFilename() == parc.params[0]) {
            file = files[i];
            break;
        }
    }
    if (file.getReceiver() != cli.getNickName())
        throw std::runtime_error("0 * :Permission Denied");
    std::fstream ofs(parc.params[1] + "/" + parc.params[0], std::fstream::out);
    std::fstream ifs(file.getPath(), std::fstream::in);
    if (ofs.is_open())  {
        std::cout << "File opened" << std::endl;
        ofs << ifs.rdbuf();
    }
    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].getFilename() == parc.params[0]) {
            files.erase(files.begin() + i);
            break;
        }
    }
}