/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:32:06 by okassimi          #+#    #+#             */
/*   Updated: 2024/02/26 10:20:51 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
using namespace std;

int main()    {
    Server      srv;
    std::string input;

    while (1337)    {
        getline(std::cin, input);
        if (input == "exit")
            return (0);
        parc(input);
    }
}