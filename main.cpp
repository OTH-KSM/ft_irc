/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:32:06 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/12 03:45:48 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"


bool Server::Signal = false;

int main(int argc, char *argv[])    {
    if (argc != 3)  {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    int port = atoi(argv[1]);
    if (port < 0 || port > 65535)   {
        std::cerr << "Error: Port number must be between 0 and 65535." << std::endl;
        return 1;
    }
    
    Server      srv(port, argv[2]);
    try {
        std::signal(SIGINT, Server::SignalHandler);
        srv.init();
    }
    catch ( std::exception &e )   {
        srv.printClients();
        srv.closeClientsFd();
        std::cout << e.what() << std::endl;
    }
}