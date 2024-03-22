/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:11:38 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 11:06:31 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incld/irc.hpp"

File::File() {}

File::File(std::string filename, std::string path, std::string sender, std::string receiver) : filename(filename), path(path), sender(sender), receiver(receiver) {}

File::~File() {}

std::string File::getFilename() const {
    return filename; 
}

std::string File::getPath() const {
    return path; 
}

std::string File::getSender() const {
    return sender; 
}

std::string File::getReceiver() const {
    return receiver; 
}


