/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:33:51 by okassimi          #+#    #+#             */
/*   Updated: 2024/02/29 13:25:00 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <exception>
#include <sstream>
#include <vector>  
#include <deque>

#define ERR_NONICKNAMEGIVEN ":No nickname given"
#define ERR_ERRONEUSNICKNAME " :Erroneus nickname"
#define ERR_NICKNAMEINUSE " :Nickname is already in use"
class   Client {
    private:
        std::string nickName;
        std::string userName;   
        // int         hostname;
        // int         
        // bool        isOperator;
        // bool        isCreator;
    public:
        Client();
        ~Client();

        void    setNickName(std::string nick);
        void    setUserName(std::string user);

        std::string    getNickName( void );
        std::string    getUserName( void );

};


class Channel {
    private:
            std::string name;
            std::string topic;

            std::vector<Client> clist;
            
        public:
            Channel(std::string uName);
            ~Channel();

            void setTopic(std::string uTopic);
            void setName(std::string uName);

            std::string getName();
            std::string getTopic();
        

        // kick  invite  topic  mode
};


class Server {
    private:
        std::string name;
        std::string password;
        std::string port;
        
    public:
        Server();
        ~Server();
};


typedef struct message {
    std::string                 prefix;           
    std::string                 cmd;
    std::deque<std::string>    params;
} t_parc;


void    parc(std::string input);


#endif