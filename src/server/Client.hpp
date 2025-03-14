/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#ifndef SRC_SERVER_CLIENT_HPP_
    #define SRC_SERVER_CLIENT_HPP_

    #include <bits/stdc++.h>
    #include <unordered_map>
    #include <memory>
    #include <string>
    #include "Socket.hpp"
    #include "IDataSocket.hpp"

namespace ftp {
class Client {
 public:
    Client() = delete;
    Client(int fd, struct sockaddr_in address, std::string rootPath);
    ~Client();
    void handleCommand(std::string commandLine);
    bool isLoggedIn() const;
    std::string getFullPath();
    std::string getRootPath();
    Socket _controlSocket;
    std::unique_ptr<IDataSocket> _dataSocket;
    std::string _username;
    std::string _password;
    std::string _currentPath;
 private:
    std::unordered_map<std::string, std::function<std::string(std::string,
        Client *)>> _commands;
    std::string _rootPath;
};
}  // namespace ftp

#endif  // SRC_SERVER_CLIENT_HPP_
