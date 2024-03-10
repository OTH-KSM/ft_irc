#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <string>




int main()
{
    int sock_listen;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);
    if((sock_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol)) == -1)
    {
        std::cout << "error in socket..." <<std::endl;
        return(0);
    }
    std::cout << "this is the ip " << hints.ai_addr << std::endl;
    if(bind(sock_listen, bind_address->ai_addr, bind_address->ai_addrlen) != 0)
    {
        std::cout << "error in bind..." << std::endl;
        return(0);
    }
    if (listen(sock_listen, 10) < 0)
    {
        std::cout << "error in listen... " << std::endl;
        return(0);
    }

    fd_set master;
    FD_ZERO(&master);
    FD_SET(sock_listen, &master);
    int max_socket = sock_listen;
    std::cout << "Waiting for connections...." << std::endl;

    while(1)
    {
        fd_set reads;
        reads = master;
        if(select(max_socket + 1, &reads, 0, 0, 0) < 0)
        {
            std::cout << "select failed..." << std::endl;
            return(0);
        }
        int i;
        for(i = 1; i <= max_socket; ++i)
        {
            if(FD_ISSET(i, &reads))
            {
                if(i == sock_listen)
                {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    int socket_client;
                    if ((socket_client = accept(sock_listen, (struct sockaddr *)&client_address, &client_len)) < 0)
                    {
                        std::cout << "error in accept... " << std::endl;
                        return(0);
                    }
                    FD_SET(socket_client, &master);
                    if(socket_client > max_socket)
                        max_socket = socket_client;

                    // Add the new client socket to the vector
                    

                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                }
            
                else
                {
                    
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    printf("%.*s", bytes_received, read);
                    if(bytes_received < 1)
                    {
                        FD_CLR(i, &master);
                        close(i);
                        continue;
                    }
                    int j;
                    std::string command(read, bytes_received);
                    // handle_client(server, command);
                    printf("%d: ", i);
                    send(i, read, bytes_received, 0);
                }
            }

        }
        
    }
    close(sock_listen);
    printf("finished\n");
    // struct sockaddr_storage client_address;
    // socklen_t client_len = sizeof(client_address);
    // int socket_client;
    // if ((socket_client = accept(sock_listen, (struct sockaddr *)&client_address, &client_len)) < 0)
    // {
    //     std::cout << "error in accept... " << std::endl;
    //     return(0);
    // }
    // char address_buffer[100];
    // getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    // printf("%s\n", address_buffer);
    // char request[1024];
    // int bytes_received = recv(socket_client, request, 1024, 0); // -1 or 0
    // printf("%.*s", bytes_received, request);


    freeaddrinfo(bind_address);
}