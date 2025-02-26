/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#include "Server.hpp"
#include <iostream>
#include <poll.h>

ftp::Server::Server(int port, std::string rootPath) :
    _socket(AF_INET, SOCK_STREAM, 0), _socketList(_socket.getSocketFd())
{
    _socket.bindSocket(port);
    _socket.listenSocket(LISTEN_BACKLOG);
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Root path: " << rootPath << std::endl;
}

ftp::Server::~Server()
{
}

int ftp::Server::pollSockets()
{
    return poll(_socketList.data(), _socketList.size(), -1);
}

void ftp::Server::updateSockets()
{
    for (std::size_t i = 0; i < _socketList.size(); i++) {
    }
}
