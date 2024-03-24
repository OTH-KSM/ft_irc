/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/24 16:52:48 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

Server::Server(int port, std::string pass) :  Servername("YourServer"), Version("1.0"), port(port), password(pass) {
	std::time_t t = std::time(0);
	this->CreateDate = std::ctime(&t);
	this->ChannelModes = "itkol";
}

Server::~Server() {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) {
		close(it->first);	
	}
}

void Server::sendInitialServerReplies(Client &cli) {
	std::string message;
	std::ostringstream portStr;
	portStr << this->port;

	message = ":" + this->Servername + " 001 " + cli.getNickName() + " :Welcome to the Internet Relay Chat network " + cli.getNickName() + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);

	message = ":" + this->Servername + " 002 " + cli.getNickName() + " :Your host is " + this->Servername + ", running version " + this->Version + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);

	message = ":" + this->Servername + " 003 " + cli.getNickName() + " :This server was created " + this->CreateDate + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);

	message = ":" + this->Servername + " 004 " + cli.getNickName() + " " + this->Servername + " " + this->Version + " NONE " + this->ChannelModes + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);

	message = ":" + this->Servername + " 005 " + cli.getNickName() + " :Try server " + this->Servername + ", port " + portStr.str() + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);


	std::vector<std::string> asciiArtLines;
	asciiArtLines.push_back("                                                                                                          ");
	asciiArtLines.push_back("                            ████                                                                          ");
	asciiArtLines.push_back("                            ████                                                           █████    ████  ");
	asciiArtLines.push_back("                                                                                           █████    ████  ");
	asciiArtLines.push_back("                                                                                           █████    ████  ");
	asciiArtLines.push_back("           ████        ██████████████     █████       ████        ████    ████    ████     █████    ████  ");
	asciiArtLines.push_back("          █████       ███████████████     █████      █████       █████   █████   █████     █████    ████  ");
	asciiArtLines.push_back("          █████       █████     █████     █████      █████       █████   █████   █████     █████    ████  ");
	asciiArtLines.push_back("          █████       ████      █████     █████      █████       █████   █████   █████     █████    ████  ");
	asciiArtLines.push_back("          █████       ████      █████     █████      █████       █████   █████   █████     █████    ████  ");
	asciiArtLines.push_back("          █████       ████      █████     █████      █████       █████   █████   █████     █████    ████  ");
	asciiArtLines.push_back("          ███████████████████████████     ██████████████████████████████████████████████████████    ████  ");
	asciiArtLines.push_back("          ███████████████████████████     ██████████████████████████████████████████████████████    ████  ");
	asciiArtLines.push_back("          ███████████████████████████    ███████████████████████████████████████████████████████    ████  ");
	asciiArtLines.push_back("          █████                          ██████                                                           ");
	asciiArtLines.push_back("         █████                           █████       ████   ████                                          ");
	asciiArtLines.push_back("       █████                           ████     	  ████   ████                                  		   ");
	asciiArtLines.push_back("     █████                           ████              █     █                                            ");
	asciiArtLines.push_back("   █████                           ████                                                                   ");
	asciiArtLines.push_back(" ████                            ███                                                                      ");


	for (std::vector<std::string>::iterator it = asciiArtLines.begin(); it != asciiArtLines.end(); ++it) {
		message = ":" + this->Servername + " 375 " + cli.getNickName() + " :" + *it + "\r\n";
		send(cli.getFd(), message.c_str(), message.size(), 0);
	}

	message = ":" + this->Servername + " 376 " + cli.getNickName() + " :End of /MOTD command" + "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);
}

/*  ____          _     _                       
* / ___|   ___  | |_  | |_    ___   _ __   ___ 
*| |  _   / _ \ | __| | __|  / _ \ | '__| / __|
*| |_| | |  __/ | |_  | |_  |  __/ | |    \__ \
* \____|  \___|  \__|  \__|  \___| |_|    |___/
* GETTERS SECTION STARTS HERE	
*/

int Server::isClientExist(std::string nick, int fd) {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)   {
		if (it->second.getNickName() == nick && it->first != fd)
			return (it->first);
	}
	return (-1);
}

Client*	Server::getClientByNick(std::string nick) {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)   {
		if (it->second.getNickName() == nick)
			return &(it->second);
	}
	return NULL;
}

Client&	Server::getClientByFd(int fd) {
	return clientMap[fd];
}

std::vector<Channel> Server::getChannels() {
	return this->channels;
}

Channel*	Server::getChannelByName(std::string name) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)  {
		if (it->getName() == name)
			return &(*it);
	}
	return NULL;
}

/*  ____   _____   _   _   _____   ____       _      _     
* / ___| | ____| | \ | | | ____| |  _ \     / \    | |    
*| |  _  |  _|   |  \| | |  _|   | |_) |   / _ \   | |    
*| |_| | | |___  | |\  | | |___  |  _ <   / ___ \  | |___ 
* \____| |_____| |_| \_| |_____| |_| \_\ /_/   \_\ |_____|
* GENERAL SECTION STARTS HERE
*/

int Server::check_valid_channel_name(std::string channel_name)
{
	std::cout << "channel_name: " << channel_name << std::endl;
    if (channel_name[0] != '#' || channel_name[0] != '&' || channel_name.size() > 200)	{
        return(0);
	}
    for(std::string::iterator ite = channel_name.begin(); ite != channel_name.end(); ite++)
    {
        if(*ite == ' ' || *ite == 7 || *ite == ',')		{
            return(0);
		}
    }
    return(1);
}

std::vector<std::string> Server::split(const std::string &s, const std::string &delim)
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
void Server::printClients() {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) {
		std::cout << "Client <" << it->first << "> Nickname: " << it->second.getNickName() << std::endl;
		std::cout << "Username: " << it->second.getUserName() << std::endl;
		std::cout << "Real Name: " << it->second.getRealName() << std::endl;
		std::cout << "Registration State: " << it->second.getRegistrationState() << std::endl;
	}
}

void Server::printChannelsAndClients() {
	std::vector<Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)	{
        std::cout << "Channel: " << (*it).getName() << std::endl;
        (*it).listUsers();
    }
}

Channel	Server::searchChannel(std::string name) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)  {
		if (it->getName() == name)
			return *it;
	}
	return *(channels.end());
}

/*  _          _      _   _   _   _    ____   _   _   ___   _   _    ____  
*  | |        / \    | | | | | \ | |  / ___| | | | | |_ _| | \ | |  / ___| 
*  | |       / _ \   | | | | |  \| | | |     | |_| |  | |  |  \| | | |  _  
*  | |___   / ___ \  | |_| | | |\  | | |___  |  _  |  | |  | |\  | | |_| | 
*  |_____| /_/   \_\  \___/  |_| \_|  \____| |_| |_| |___| |_| \_|  \____| 
*/

void    Server::init()  {
	this->port = 8080;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo *bind_address;
	getaddrinfo("10.11.1.11", "8080", &hints, &bind_address);

	if((this->SersocketFD = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol)) == -1)
		throw std::runtime_error("Error in Socket");
	int reuse = 1;
	if (setsockopt(this->SersocketFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        throw std::runtime_error("Error in setsockopt");
	if(bind(this->SersocketFD, bind_address->ai_addr, bind_address->ai_addrlen) != 0)
		throw std::runtime_error("Error in Bind");
	if (listen(this->SersocketFD, SOMAXCONN) < 0)
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
					this->clientMap.insert(std::make_pair(socket_client, cli));
					char address_buffer[100];
					getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
					std::cout << "Client <" << socket_client << "> Connected" << std::endl;
				}
				else
				{
					char read[1024];
					int bytes_received = recv(i, read, 1024, 0);
					std::cout << bytes_received << std::endl;
					if(bytes_received < 1)	{
						handleQuitCommand(*this, i, master, true);
						continue ;
					}
					read[bytes_received] = 0;
					this->input.append(read);
					std::cout << this->input << std::endl;
					if (this->input.find("\n") != std::string::npos)	{
						try
						{
							Server::parc(this->input, clientMap[i]);
						}
						catch(const std::exception& e)
						{
							std::string cont = e.what();
							std::cout << this->Servername << std::endl;
							std::string message = ":YourServer " + cont + "\r\n";
							send(i, message.c_str(), message.size(), 0);
						}
						this->input.clear();
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
	
	size_t prefixEnd = message.find(":", 2);
	if (prefixEnd != std::string::npos) {
		std::string prefix = message.substr(prefixEnd + 1, message.length() - 2);
		message.erase(prefixEnd, message.length());
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
	
	/*	PRINT COMMANDS	*/
	std::deque<std::string>::iterator it;
	int i = 0;
		std::cout << "prefix -> " << parc.prefix << std::endl;
		std::cout << "comand -> " << parc.cmd << std::endl;
	for (it = parc.params.begin(); it != parc.params.end(); it++)   {
		std::cout << "param  " << i++ << " -> " << *it << std::endl;
	}
	
	/*	HANDLE COMMANDS	*/
	if (parc.cmd == "PASS" || parc.cmd == "NICK" || parc.cmd == "USER")	{
		if (parc.cmd == "PASS")
			handlePassCommand(parc, cli);
		else if (parc.cmd == "NICK")
			handleNickCommand(parc, cli);
		else if (parc.cmd == "USER")
			handleUserCommand(parc, cli);
	}
    else if (parc.cmd == "JOIN")
        handleJoinCommand(parc, cli);
	else if (parc.cmd == "PRIVMSG")
		handlePrivmsgCommand(parc, cli);
	else if (parc.cmd == "MODE")
		handleModeCommand(parc, cli);
	else if (parc.cmd == "PONG")
		;
	else if (parc.cmd == "PRINT")
		printChannelsAndClients();
	else if (parc.cmd == "INVITE")
		handleInviteCommand(parc, cli);
	else if (parc.cmd == "BOTOX")
		Bot(parc, cli);
	else if (parc.cmd == "TOPIC")
		handleTopicCommand(parc, cli);
	else if (parc.cmd == "KICK")
		handleKickCommand(parc, cli);
	else if (parc.cmd == "SENDFILE")
		handleSendFileCommand(parc, cli);
	else if (parc.cmd == "GETFILE")
		handleGetFileCommand(parc, cli);
	else
		throw std::runtime_error("421 * " + parc.cmd + " :Unknown command");
}


