/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include <bits/stdc++.h>
    #include "Socket.hpp"

namespace ftp {
    class Client {
        public:
            Client() = delete;
            Client(int fd, struct sockaddr_in address);
            ~Client();
            void handleCommand(std::string commandLine);
            Socket _socket;
        private:
            std::unordered_map<std::string,
                std::function<std::string(std::string, Client &)>> _commands;
    };
}

#endif /* !CLIENT_HPP_ */