/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/13 18:32:11 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

/************************ Server ************************/

Server::Server(int port, std::string pass) :  Servername("YourServer"), port(port), password(pass) {
    
}

Server::~Server() {
        
}

std::string Server::getServerName( void )   {
    return this->Servername;
}

int Server::getFd( void )   {
    return this->SersocketFD;
}

void Server::printClients() {
    for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) {
        std::cout << "Client <" << it->first << "> Nickname: " << it->second.getNickname() << std::endl;
        std::cout << "Username: " << it->second.getUsername() << std::endl;
        std::cout << "Real Name: " << it->second.getRealName() << std::endl;
        std::cout << "Registration State: " << it->second.getRegistrationState() << std::endl;
    }
}

void Server::closeClientsFd() {
    for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) {
        close(it->first);
    }
    close(this->SersocketFD);
}

int Server::getClientNick(std::string nick, int fd) {
    for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)   {
        if (it->second.getNickname() == nick && it->first != fd)
            return (it->first);
    }
    return (-1);
}

void    Server::sendOneToOne(Client& cli, std::string dest, std::string message)    {
    int destFd = getClientNick(dest, cli.getFd());
    if (destFd == -1)
        throw std::runtime_error(dest + " :No such nick/channel\n");
    std::string newmsg = ":" + cli.getNickname() + " PRIVMSG " + dest + " : " + message + "\r\n";
    
    send(destFd, newmsg.c_str(), newmsg.size(), 0);
}

void    Server::sendToChannel(std::string dest, std::string message) {
    (void)dest;
    (void)message;
}


Channel	Server::searchChannel(std::string name) {
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)  {
        if (it->getName() == name)
            return *it;
    }
    return *(channels.end());
}

void    Server::init()  {
    this->port = 8080;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo("10.11.2.10", "8080", &hints, &bind_address);

    if((this->SersocketFD = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol)) == -1)
        throw std::runtime_error("Error in Socket");
    if(bind(this->SersocketFD, bind_address->ai_addr, bind_address->ai_addrlen) != 0)
        throw std::runtime_error("Error in Bind");
    if (listen(this->SersocketFD, 10) < 0)
        throw std::runtime_error("Error in Listen");


    fd_set master;
    FD_ZERO(&master);
    FD_SET(this->SersocketFD, &master);
    int max_socket = this->SersocketFD;
    std::cout << "Waiting for connections...." << std::endl;


    while(this->Signal == false)
    {
        fd_set reads;
        reads = master;
        if(select(max_socket + 1, &reads, 0, 0, 0) < 0)
            throw std::runtime_error("Error in Select");
        int i;
        for(i = 0; i <= max_socket; ++i)
        {
            if(FD_ISSET(i, &reads))
            {
                if(i == this->SersocketFD)
                {
                    Client  cli;
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    int socket_client;
                    if ((socket_client = accept(this->SersocketFD, (struct sockaddr *)&client_address, &client_len)) < 0)
                        throw std::runtime_error("Error in Accept");
                    FD_SET(socket_client, &master);
                    if(socket_client > max_socket)
                        max_socket = socket_client;
                    cli.setFd(socket_client);
                    // Add the new client socket to the vector
                    this->clientMap.insert(std::make_pair(socket_client, cli));
                    send(socket_client, "ahlan bik o marhba bik\r\n", 24, 0);
                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                    std::cout << "Client <" << socket_client << "> Connected" << std::endl;
                }
                else
                {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if(bytes_received < 1)
                    {
                        FD_CLR(i, &master);
                        close(i);
                        continue;
                    }
                    read[bytes_received] = 0;
                    if (!strcmp(read, "exit\n"))
                        throw std::runtime_error("ohh magad");
                    std::string str(read);
                    try
                    {
                        Server::parc(str, clientMap[i]);
                    }
                    catch(const std::exception& e)
                    {
                        std::string cont = e.what();
                        std::cout << this->Servername << std::endl;
                        std::string message = ":YourServer " + cont + "\r\n";
                        send(i, message.c_str(), message.size(), 0);
                    }
                }
            }
        }
    }
}

// message = ":" + this->Servername + " 000 " + clientMap[i].getNickname() + cont + "\r\n";
// message = ":" + this->Servername + " 111   :NICKNAME: " + cli.getNickname() + "\r\n";

/************************ GENERAL ************************/

void    Server::parc(std::string message, Client& cli) {
    t_parc      parc;
    std::string temp;
    std::stringstream cc;

    message.erase(message.length() - 1);
    size_t prefixEnd = message.find(":", 2);
    if (prefixEnd != std::string::npos) {
        std::string prefix = message.substr(prefixEnd + 1, message.length() - 2);
        message.erase(prefixEnd, message.length() - 2);
        cc.str(message);
        while (cc >> temp)  {
            parc.params.push_back(temp);
        }
        parc.params.push_back(prefix);
    }
    cc.str(message);
    while (cc >> temp)  {
        parc.params.push_back(temp);
    }
    if (!parc.params.empty() && !parc.params.front().empty() &&  parc.params.front()[0] == ':')  {
        parc.prefix = parc.params.front();
        parc.params.pop_front();
    }
    if (!parc.params.empty())    {
        parc.cmd = parc.params.front();
        parc.params.pop_front();
    }
    if (parc.params.size() > 15)
        std::runtime_error("Error: More than 15 parameters are not allowed.");
    
    std::deque<std::string>::iterator it;
    int i = 0;
        std::cout << "prefix -> " << parc.prefix << std::endl;
        std::cout << "comand -> " << parc.cmd << std::endl;
    for (it = parc.params.begin(); it != parc.params.end(); it++)   {
        std::cout << "param  " << i++ << " -> " << *it << std::endl;
    }
    
    int state = cli.getRegistrationState();
    std::string nick;
    if (parc.cmd == "PASS") { // 0 -> pass  
        if (state != 1) {
            if (state != 0) {
                if (state == 2)
                    throw std::runtime_error("421 * :Unexpected command DO: NICK");
                throw std::runtime_error("462 * :You may not reregister");
            }
            if (parc.params.empty())
                throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
            if (parc.params.front() != this->password)
                throw std::runtime_error("464 * :Incorrect password provided with PASS command.");
            cli.setRegistrationState(1); // 1 -> nick
        }
    }
    else if (parc.cmd == "NICK") {
        if (state != 1) {
            if (state == 0)
                throw std::runtime_error("451 * :You have not registered");
            throw std::runtime_error("451 * :You have not registered");
        }
        nick = parc.params.front();
        if (nick.empty())
            throw std::runtime_error("431 * :No nickname given");
        if (nick.length() > 9 || nick.find_first_of(" ,*?!@.#") != std::string::npos)
            throw std::runtime_error("432 * " + nick + " :Erroneus nickname");
        cli.setNickName(nick);
        cli.setRegistrationState(2); // 2 -> user
    }
    else if (parc.cmd == "USER")    {
        if (state != 2) {
            if (state == 0 || state == 1)
                throw std::runtime_error("451 * :You have not registered");
            throw std::runtime_error("462 * :You may not reregister");
        }
        if (parc.params.size() < 4)
             throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
        cli.setUsername(parc.params[0]);
        cli.setRealName(parc.params[3]);
        cli.setRegistrationState(3);
    }
    else if (parc.cmd == "SHOWME")  {
        std::string message;
        
        message = ":" + this->Servername + " PRIVMSG " + cli.getNickname() + " :NICKNAME: " + cli.getNickname() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " PRIVMSG " + cli.getNickname() + " :USERNAME: " + cli.getUsername() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " PRIVMSG " + cli.getNickname() + " :REALNAME: " + cli.getRealName() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " PRIVMSG " + cli.getNickname() + " :REGISTRA: " + std::to_string(cli.getRegistrationState()) + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " PRIVMSG " + cli.getNickname() + " :CLIENTFD: " + std::to_string(cli.getFd()) + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
    }
    else if (parc.cmd == "PRIVMSG") {
        if (state != 3)
            throw std::runtime_error("451 * :You have not registered");
        if (parc.params.size() < 2)
            throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
        sendOneToOne(cli, parc.params[0], parc.params[1]);
    }
    else
        send(cli.getFd(), "ACH HAD L INPUT", 15, 0);
}

