/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 12:23:35 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 17:22:10 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

Botox::Botox()   {
    this->name = "Botox";
    
    this->jokes.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.\r\n");
    this->jokes.push_back("What do you call a fake noodle? An impasta.\r\n");
    this->jokes.push_back("Why did the coffee file a police report? It got mugged.\r\n");
    this->jokes.push_back("How does a penguin build its house? Igloos it together.\r\n");
    this->jokes.push_back("Dad, did you get a haircut? No, I got them all cut.\r\n");
    this->jokes.push_back("What do you call a belt made out of watches? A waist of time.\r\n");
    this->jokes.push_back("Why don't skeletons fight each other? They don't have the guts.\r\n");


    this->quotes.push_back("The greatest glory in living lies not in never falling, but in rising every time we fall. - Nelson Mandela\r\n");
    this->quotes.push_back("The way to get started is to quit talking and begin doing. - Walt Disney\r\n");
    this->quotes.push_back("Your time is limited, so don't waste it living someone else's life. - Steve Jobs\r\n");
    this->quotes.push_back("If life were predictable it would cease to be life, and be without flavor. - Eleanor Roosevelt\r\n");
    this->quotes.push_back("If you look at what you have in life, you'll always have more. - Oprah Winfrey\r\n");
    this->quotes.push_back("If you set your goals ridiculously high and it's a failure, you will fail above everyone else's success. - James Cameron\r\n");
    this->quotes.push_back("Life is what happens when you're busy making other plans. - John Lennon\r\n");

    this->facts.push_back("The shortest war in history was between Zanzibar and England in 1896. Zanzibar surrendered after 38 minutes.\r\n");
    this->facts.push_back("The oldest your mom joke was discovered on a 3,500 year old Babylonian tablet.\r\n");
    this->facts.push_back("The largest snowflake ever recorded reportedly measured 15 inches across.\r\n");
    this->facts.push_back("The first American film to show a toilet being flushed on screen was Alfred Hitchcock's Psycho.\r\n");
    this->facts.push_back("The first    product to have a bar code was Wrigley's gum.\r\n");
    this->facts.push_back("The first owner of the Marlboro company died of lung cancer.\r\n");
    this->facts.push_back("The first domain name ever registered was Symbolics.com.\r\n");
}

Botox::~Botox()  {
    this->jokes.clear();
    this->quotes.clear();
    this->facts.clear();
}

std::string Botox::getJoke( void ) {
    return (this->jokes[rand() % this->jokes.size()]);
}

std::string Botox::getQuote( void ) {
    return (this->quotes[rand() % this->quotes.size()]);
}

std::string Botox::getfacts( void ) {
    return (this->facts[rand() % this->facts.size()]);
}

void    Server::Bot(t_parc &parc, Client &cli)  {
    if (parc.params.size() != 2) {
        throw std::runtime_error("461 * " + parc.cmd + " :Incorrect number of p00arameters. Expected " + std::to_string(2) + ", got " + std::to_string(parc.params.size()));
    }
    Client *target = getClientByNick(parc.params[0]);
    if (!target)    {
        throw std::runtime_error("461 " + cli.getNickName() + " " + parc.cmd + " :No such Nick");
    }
    std::string intro;
    std::string message;
    Botox bot;
    if (!parc.params[1].empty()) {
        if (parc.params[1] == "JOKE")   {
            intro = "Hello, " + target->getNickName() + "! I'm BMOBot. Someone thought you could use a smile today, so here's a joke for you.";
            message = ":BOtOX  NOTICE " + target->getNickName() + " :" + intro + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
                        message = ":BOtOX  NOTICE " + target->getNickName() + " :" + bot.getJoke() + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
        }
        else if (parc.params[1] == "MOTIVE")    {
            intro = "Hello, " + cli.getNickName() + "! I'm BMOBot. Someone thought you could use a smile today, so here's a motivational quote for you.";
            message = ":BOtOX  NOTICE " + target->getNickName() + " :" + intro + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
            message = ":BOtOX  NOTICE " + target->getNickName() + " :" + bot.getQuote() + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
        }
        else if (parc.params[1] == "FACT")  {
            intro = "Hello, " + cli.getNickName() + "! I'm BMOBot. Someone thought you could use a smile today, so here's a fact for you.\r\n" + bot.getfacts();
            message = ":BOtOX  NOTICE " + target->getNickName() + " :" + intro + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
            message = ":BOtOX  NOTICE " + target->getNickName() + " :" + bot.getfacts() + "\r\n";
            send(target->getFd(), message.c_str(), message.size(), 0);
        }
    }
}