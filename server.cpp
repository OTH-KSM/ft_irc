/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 09:47:48 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/15 01:21:33 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

Server::Server(int port, std::string pass) :  Servername("YourServer"), Version("1.0"), port(port), password(pass) {
	std::time_t t = std::time(0);   // get time now
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
	asciiArtLines.push_back(" _____");
	asciiArtLines.push_back("|___ /    __ _   _ __ ___     ___       ___    ___   _ __  __   __   ___ _ __");
	asciiArtLines.push_back("  |_ \\   / _` | | '_ ` _ \\   / _ \\     / __|  / _ \\ | '__| \\ \\ / /  / _ \\ '__|");
	asciiArtLines.push_back(" ___) | | (_| | | | | | | | | (_) |    \\__ \\ |  __/ | |     \\ V /  |  __/ |");
	asciiArtLines.push_back("|____/   \\__,_| |_| |_| |_|  \\___/     |___/  \\___| |_|      \\_/    \\___|_|");

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

std::string Server::getServerName( void )   {
	return this->Servername;
}

int Server::getFd( void )   {
	return this->SersocketFD;
}

int Server::isClientExist(std::string nick, int fd) {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)   {
		if (it->second.getNickName() == nick && it->first != fd)
			return (it->first);
	}
	return (-1);
}

Client&	Server::getClientByNick(std::string nick) {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it)   {
		if (it->second.getNickName() == nick)
			return (it->second);
	}
	throw std::runtime_error("401 * " + nick + " :No such Nickname");
}

Client&	Server::getClientByFd(int fd) {
	return clientMap[fd];
}

/*             _   _                _   _  __ _           _   _             
*  __ _ _   _| |_| |__   ___ _ __ | |_(_)/ _(_) ___ __ _| |_(_) ___  _ __  
* / _` | | | | __| '_ \ / _ \ '_ \| __| | |_| |/ __/ _` | __| |/ _ \| '_ \ 
*| (_| | |_| | |_| | | |  __/ | | | |_| |  _| | (_| (_| | |_| | (_) | | | |
* \__,_|\__,_|\__|_| |_|\___|_| |_|\__|_|_| |_|\___\__,_|\__|_|\___/|_| |_|
* AUTHENTIFICATION SECTION STARTS HERE
*/

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
	cli.setNickName(nick);
	if (state != 3)
		cli.setRegistrationState(2); // 2 -> user
}

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

/*  ___    _____   _   _   _____   ____       ____    ___    __  __   __  __      _      _   _   ____    ____  
* / _ \  |_   _| | | | | | ____| |  _ \     / ___|  / _ \  |  \/  | |  \/  |    / \    | \ | | |  _ \  / ___| 
*| | | |   | |   | |_| | |  _|   | |_) |   | |     | | | | | |\/| | | |\/| |   / _ \   |  \| | | | | | \___ \ 
*| |_| |   | |   |  _  | | |___  |  _ <    | |___  | |_| | | |  | | | |  | |  / ___ \  | |\  | | |_| |  ___) |
* \___/    |_|   |_| |_| |_____| |_| \_\    \____|  \___/  |_|  |_| |_|  |_| /_/   \_\ |_| \_| |____/  |____/ 
* OTHER COMMANDS SECTION STARTS HERE
*/

void	Server::handleWhoisCommand(t_parc &parc, Client& cli)	{
		if (parc.params[0].empty())
			throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
		Client targetClient = getClientByNick(parc.params[0]);

		std::string nick = targetClient.getNickName(); 
		std::string user = targetClient.getUserName(); 
		// std::string host = targetClient.getHostname(); // The hostname of the client
		std::string realName = targetClient.getRealName(); 
		std::string serverName = this->Servername; 
		std::string serverInfo = "The best IRC server Ever";

		std::string message = ":" + this->Servername + " 311 " + cli.getNickName() + " " + nick + " " + user + " " + "10.11.1.11" + " * :" + realName + "\r\n";
		send(cli.getFd(), message.c_str(), message.size(), 0);

		message = ":" + this->Servername + " 312 " + cli.getNickName() + " " + nick + " " + serverName + " :" + serverInfo + "\r\n";
		send(cli.getFd(), message.c_str(), message.size(), 0);

		message = ":" + this->Servername + " 318 " + cli.getNickName() + " " + nick + " :End of WHOIS list\r\n";
		send(cli.getFd(), message.c_str(), message.size(), 0);
}

void	Server::handlePrivmsgCommand(t_parc &parc, Client& cli)	{
	int state = cli.getRegistrationState();
	if (state != 3)
		throw std::runtime_error("451 * :You have not registered");
	if (parc.params.size() < 2)
		throw std::runtime_error("461 * " + parc.cmd + " :Not enough parameters");
	sendOneToOne(cli, parc.params[0], parc.params[1]);
}

/*  ____   _____   _   _   _____   ____       _      _     
* / ___| | ____| | \ | | | ____| |  _ \     / \    | |    
*| |  _  |  _|   |  \| | |  _|   | |_) |   / _ \   | |    
*| |_| | | |___  | |\  | | |___  |  _ <   / ___ \  | |___ 
* \____| |_____| |_| \_| |_____| |_| \_\ /_/   \_\ |_____|
* GENERAL SECTION STARTS HERE
*/

void Server::printClients() {
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) {
		std::cout << "Client <" << it->first << "> Nickname: " << it->second.getNickName() << std::endl;
		std::cout << "Username: " << it->second.getUserName() << std::endl;
		std::cout << "Real Name: " << it->second.getRealName() << std::endl;
		std::cout << "Registration State: " << it->second.getRegistrationState() << std::endl;
	}
}



void    Server::sendOneToOne(Client& cli, std::string dest, std::string message)    {
	int destFd = isClientExist(dest, cli.getFd());
	if (destFd == -1)
		throw std::runtime_error(dest + " :No such Nickname\n");
	std::string newmsg = ":" + cli.getNickName() + " PRIVMSG " + dest + " : " + message + "\r\n";
	
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
	getaddrinfo("10.11.1.11", "8080", &hints, &bind_address);

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

/************************ GENERAL ************************/

void    Server::parc(std::string message, Client& cli) {
	t_parc      parc;
	std::string temp;
	std::stringstream cc;

	message.erase(message.size() - 2);
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
	else if (parc.cmd == "WHOIS")
		handleWhoisCommand(parc, cli);
	else if (parc.cmd == "PRIVMSG")
		handlePrivmsgCommand(parc, cli);
	else if (parc.cmd == "PING" || parc.cmd == "PONG")	{
		
	}
}


