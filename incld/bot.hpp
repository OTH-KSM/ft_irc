/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:43:16 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/20 17:02:43 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "irc.hpp"


class   Botox   {
    private:
        std::string              name;
        std::vector<std::string> jokes;
        std::vector<std::string> quotes;
        std::vector<std::string> facts;
    public:
        Botox();
        ~Botox();

        std::string getJoke( void );
        std::string getQuote( void );
        std::string getfacts( void ); 
};


#endif