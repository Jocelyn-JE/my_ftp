/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#ifndef SRC_CLIENT_CLIENT_HPP_
    #define SRC_CLIENT_CLIENT_HPP_

    #include <bits/stdc++.h>
    #include <unordered_map>
    #include <memory>
    #include <string>
    #include "sockets/Socket.hpp"
    #include "sockets/IDataSocket.hpp"

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
    class Commands {
     public:
        static std::string doUser(std::string commandLine, Client *client);
        static std::string doPass(std::string commandLine, Client *client);
        static std::string doCwd(std::string commandLine, Client *client);
        static std::string doCdup(std::string commandLine, Client *client);
        static std::string doQuit(std::string commandLine, Client *client);
        static std::string doDelete(std::string commandLine, Client *client);
        static std::string doPwd(std::string commandLine, Client *client);
        static std::string doPasv(std::string commandLine, Client *client);
        static std::string doPort(std::string commandLine, Client *client);
        static std::string doNoop(std::string commandLine, Client *client);
        static std::string doHelp(std::string commandLine, Client *client);
        static std::string doRetr(std::string commandLine, Client *client);
        static std::string doList(std::string commandLine, Client *client);
        static std::string doStor(std::string commandLine, Client *client);
    };
 private:
    std::unordered_map<std::string, std::function<std::string(std::string,
        Client *)>> _commands;
    std::string _rootPath;
};
}  // namespace ftp

#endif  // SRC_CLIENT_CLIENT_HPP_
