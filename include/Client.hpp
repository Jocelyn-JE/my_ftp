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
            Client(int fd, struct sockaddr_in address, std::string rootPath);
            ~Client();
            void handleCommand(std::string commandLine);
            bool isLoggedIn() const;
            std::string getFullPath();
            Socket _socket;
            std::string _username;
            std::string _password;
            std::string _currentPath;
        private:
            std::unordered_map<std::string,
                std::function<std::string(std::string, Client &)>> _commands;
            std::string _rootPath;
    };
}

#endif /* !CLIENT_HPP_ */