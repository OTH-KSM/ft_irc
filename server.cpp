/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/12 03:50:13 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
/************************ Extra ************************/
int check_valid_channel_name(std::string channel_name)
{
    if (channel_name[0] != '#' || channel_name[0] != '@' || channel_name.size() > 200)
        return(0);
    for(std::string::iterator ite = channel_name.begin(); ite != channel_name.end(); ite++)
    {
        if(*ite == ' ' || *ite == 7 || *ite == ',')
            return(0);
    }
    return(1);
}

std::vector<std::string> split(const std::string &s, const std::string &delim)
{
    std::vector<std::string> ret;
    size_t pos_start = 0, pos_end, delim_len = delim.length();
    std::string token;
    while((pos_end = s.find(delim, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        ret.push_back(token);
    }
    ret.push_back(s.substr(pos_start));
    return ret;
}
/************************ Server ************************/

Server::Server(int port, std::string pass) :  Servername("YourServer"), port(port), password(pass) {
    
}

Server::~Server() {
        
}

// void Server::SignalHandler(int signum)
// {
//     (void)signum;
//     std::cout << std::endl << "Signal Received!" << std::endl;
//     this->Signal = true;
// }

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
    std::string newmsg = ":" + cli.getNickname() + " PRIVMSG " + dest + " : "+ message + "\r\n";
    
    send(destFd, newmsg.c_str(), newmsg.size(), 0);
}

void    Server::sendToChannel(Client &cli, std::string dest, std::string message) {
    for(std::vector<Channel>::iterator ite = channels.begin(); ite != channels.end(); ite++)
    {
        if((*ite).getName() == dest)
        {
            std::vector<Client> users = (*ite).get_users();
            for(std::vector<Client>::iterator ite = users.begin(); ite != users.end(); ite++)
            {
                int destFd = (*ite).getFd();
                if (destFd == -1)
                    throw std::runtime_error(dest + " :No such nick/channel\n");
                std::string newmsg = ":" + cli.getNickname() + " PRIVMSG " + dest + " : "+ message + "\r\n";
                send(destFd, newmsg.c_str(), newmsg.size(), 0);
            }
        }
    }
}


int	Server::handleChannel(std::vector<std::string> split_channels, std::vector<std::string> split_keys, Client &cli) {

    std::cout << "handlechannel is working" << std::endl;
    for(size_t i = 0; i < split_channels.size(); i++)
    {
        if(check_valid_channel_name(split_channels[i]))
            continue;
        for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++) {// gotta check keys  
            if ((*it).getName() == split_channels[i])
            {
                if((*it).CheckClientExistInChannel(cli) == 0)
                {
                    std::cout << "new client was added to " << (*it).getName() << std::endl;
                    (*it).addClientToChannel(cli, i, split_keys);
                    std::cout << "channels " << channels.size() << " " << "channel_clients in " << (*it).getName() << " is " << (*it).getClientsNumber() << std::endl;
                    return 1; //channel exist
                }
                else
                {
                    std::cout << "channels " << channels.size() << " " << "channel_clients in " << (*it).getName() << " is " << (*it).getClientsNumber() << std::endl;
                    return 0; // client already exist in channel;
                }
            }
        }
        std::cout << "new channel was created " << std::endl;
        if(split_keys.size() >= i + 1)
        {
            Channel new_channel(split_channels[i], split_keys[i]);
            new_channel.addClientToChannel(cli, i, split_keys); // this client should become a client operator u gotta handle it.
            channels.push_back(new_channel);
        }
        else
        {
            Channel new_channel(split_channels[i]);
            new_channel.addClientToChannel(cli, i, split_keys); // this client should become a client operator u gotta handle it.
            channels.push_back(new_channel);
        }
        std::cout << "channels " << channels.size() << " " << "channel_clients in " << channels[i].getName() << " is " << channels[i].getClientsNumber() << std::endl;
    }
    return 0;
}

void    Server::init()  {
    this->port = 8080;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    if((this->SersocketFD = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol)) == -1)
        throw std::runtime_error("Error in Socket");
    if(bind(this->SersocketFD, bind_address->ai_addr, bind_address->ai_addrlen) != 0)
        throw std::runtime_error("Error in Bind");
    if (listen(this->SersocketFD, 10) < 0) // gotta change the 10 later
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
                    read[bytes_received] = 0; // setting backslash 0 at the end
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
                        std::string message = ":" + this->Servername + " 000 " + clientMap[i].getNickname() + cont + "\r\n";
                        send(i, message.c_str(), message.size(), 0);
                    }
                }
            }
        }
    }
}

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
    if (parc.cmd == "PASS") {
        if (state != 0)  // 0 -> pass
            throw std::runtime_error(" :Unexpected PASS command.");
        if (parc.params.empty())
            throw std::runtime_error(parc.cmd + " :Not enough parameters");
        if (parc.params.front() != this->password)
            throw std::runtime_error(" :Incorrect password provided with PASS command." + this->password);
        cli.setRegistrationState(1); // 1 -> nick
    }
    else if (parc.cmd == "NICK") {
        if (state != 1) 
            throw std::runtime_error(" :Unexpected NICK command.");
        nick = parc.params.front();
        if (nick.empty())
            throw std::runtime_error(" :No nickname given");
        if (nick.length() > 9 || nick.find_first_of(" ,*?!@.#") != std::string::npos)
            throw std::runtime_error(nick + " :Erroneus nickname");
        cli.setNickName(nick);
        cli.setRegistrationState(2); // 2 -> user
    }
    else if (parc.cmd == "USER")    {
        if (state != 2)
            throw std::runtime_error(" :Unexpected USER command.");
        if (parc.params.size() < 4)
            throw std::runtime_error(parc.cmd + " :Not enough parameters");
        cli.setUsername(parc.params[0]);
        cli.setRealName(parc.params[3]);
        cli.setRegistrationState(3);
    }
    else if (parc.cmd == "JOIN")    {
        if (state != 3)
            throw std::runtime_error(" :Unexpected JOIN command");
        if (parc.params.size() < 1)
            throw std::runtime_error(parc.cmd + " :Not enough parameters");
        std::vector<std::string> split_channels;
        std::vector<std::string> split_keys;
        if (parc.params.size() >= 1)
            split_channels = split(parc.params[0], ",");
        if(parc.params.size() >= 2)
            split_keys = split(parc.params[1], ",");
        handleChannel(split_channels, split_keys, cli);
    }
    else if (parc.cmd == "SHOWME")  {
        std::string message;
        
        message = ":" + this->Servername + " 111 " + cli.getNickname() + " :NICKNAME: " + cli.getNickname() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " 111 " + cli.getNickname() + " :USERNAME: " + cli.getUsername() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " 111 " + cli.getNickname() + " :REALNAME: " + cli.getRealName() + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " 111 " + cli.getNickname() + " :REGISTRA: " + std::to_string(cli.getRegistrationState()) + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        message = ":" + this->Servername + " 111 " + cli.getNickname() + " :CLIENTFD: " + std::to_string(cli.getFd()) + "\r\n";
        send(cli.getFd(), message.c_str(), message.size(), 0);
        
    }
    else if (parc.cmd == "PRIVMSG") {
        if (state != 3)
            throw std::runtime_error(" :Unexpected PRIVMSG command");
        if (parc.params.size() < 2)
            throw std::runtime_error(parc.cmd + " :Not enough parameters");
        if (parc.params[0][0] == '#' || parc.params[0][0] == '&')
            sendToChannel(cli, parc.params[0], parc.params[1]);
        else
        {
            std::cout << std::endl << "<" << parc.params[1] << ">" << std::endl;
            sendOneToOne(cli, parc.params[0], parc.params[1]);
        }
    }
    else
        send(cli.getFd(), "ACH HAD L INPUT", 15, 0);
}

