/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okassimi <okassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 04:05:16 by okassimi          #+#    #+#             */
/*   Updated: 2024/03/22 11:06:46 by okassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP

#include "common.hpp"

class File {
    private:
        std::string filename;
        std::string path;
        std::string sender;
        std::string receiver;
    public:
        File();
        File(std::string filename, std::string path, std::string sender, std::string receiver);
        ~File();

        std::string getFilename() const;
        std::string getPath() const;
        std::string getSender() const;
        std::string getReceiver() const;
};

#endif