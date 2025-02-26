/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#include "Server.hpp"
#include <iostream>
#include <poll.h>
#include <arpa/inet.h>

static std::vector<ftp::Socket> initSocketList()
{
    std::vector<ftp::Socket> list;

    list.emplace_back(AF_INET, SOCK_STREAM, 0);
    return list;
}

ftp::Server::Server(int port, std::string rootPath) :
    _socketList(initSocketList()), _socketPollList(_socketList[0].getSocketFd())
{
    _socketList[0].bindSocket(port);
    _socketList[0].listenSocket(LISTEN_BACKLOG);
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Root path: " << rootPath << std::endl;
}

ftp::Server::~Server()
{
}

int ftp::Server::pollSockets()
{
    return poll(_socketPollList.data(), _socketList.size(), -1);
}

void ftp::Server::updateSockets()
{
    for (std::size_t i = 0; i < _socketList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN && _socketPollList[i].fd ==
            _socketList[0].getSocketFd()) {
            handleConnection();
        } else if (_socketPollList[i].revents & POLLIN & POLLOUT) {
            
        }
    }
}

void ftp::Server::handleConnection()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(_socketList[0].getSocketFd(),
        (struct sockaddr *) &client_addr, &client_addr_size);

    this->_socketList.emplace_back(client_socket, client_addr);
    this->_socketPollList.addSocket(client_socket, POLLIN & POLLOUT);
    printf("%s:%d connected fd: %d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port), client_socket);
    _socketList.back().writeToSocket("220 Service ready for new user.");
}
