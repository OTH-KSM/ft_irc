/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:33:51 by okassimi          #+#    #+#             */
/*   Updated: 2024/02/26 10:14:27 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>
#include <vector>  
#include <deque>

class   Client {
    private:
        std::string nickName;
        std::string userName;
        std::string fullName;
    public:
        Client();
        ~Client();

        void    setNickName(std::string nick);
        void    setUserName(std::string user);
        void    setFullName(std::string full);

        std::string    getNickName( void );
        std::string    getUserName( void );
        std::string    getFullName( void );
};


class Channel {
    private:
        std::string name;
        std::string topic;
        
        std::map<int, Client> list;
    public:
        Channel(std::string uName);
        ~Channel();

        void    setTopic(std::string uTopic);
        void    setName(std::string uName);

        std::string    getName( void );
        std::string    getTopic( void );
        
        
        
};


class Server {
    private:
        std::string name;
        
        std::vector<Channel> channelList;
    public:
        Server();
        ~Server();
        
        
};

void    parc(std::string input);


#endif