/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:32:06 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/09 21:01:14 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

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
        srv.init();
    }
    catch ( std::exception &e )   {
        // clearClients and Fds
        srv.printClients();
        std::cout << e.what() << std::endl;
    }
}