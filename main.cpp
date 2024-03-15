/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:32:06 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/14 16:47:36 by okassimi         ###   ########.fr       */
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
    
    try {
        Server      srv(port, argv[2]);
        std::signal(SIGINT, Server::SignalHandler);
        srv.init();
    }
    catch ( std::exception &e )   {
        std::cout << e.what() << std::endl;
        std::cout << "Server operation has been suspended. Please check the error message above and try again." << std::endl;
    }
}
