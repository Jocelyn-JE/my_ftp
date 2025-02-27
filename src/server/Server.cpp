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
#include <unistd.h>
#include <string.h>

static std::vector<std::unique_ptr<ftp::Socket>> initSocketList()
{
    std::vector<std::unique_ptr<ftp::Socket>> list;

    list.push_back(std::make_unique<ftp::Socket>(AF_INET, SOCK_STREAM, 0));
    return list;
}

ftp::Server::Server(int port, std::string rootPath) :
    _socketList(initSocketList()), _socketPollList(_socketList[0]->getSocketFd())
{
    _socketList[0]->bindSocket(port);
    _socketList[0]->listenSocket(LISTEN_BACKLOG);
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
    std::string socketStr;

    for (std::size_t i = 0; i < _socketList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN && _socketPollList[i].fd ==
            _socketList[0].getSocketFd()) {
            handleConnection();
        } else if (_socketPollList[i].revents & POLLIN & POLLOUT) {
            try {
                socketStr = _socketList[i].readFromSocket();
            } catch(const ftp::Socket::SocketError &e) {
                std::cerr << "Client " << _socketList[i].getSocketFd() <<
                    " disconnected\n" << std::endl;
            }
        }
        if (_socketPollList[i].revents & POLLHUP) {
            _socketList.erase(_socketList.begin() + i);
            _socketPollList.erase(_socketPollList.begin() + i);
        }
    }
}

void ftp::Server::handleConnection()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(_socketList[0]->getSocketFd(),
        (struct sockaddr *) &client_addr, &client_addr_size);

    this->_socketList.push_back(std::make_unique<Socket>(client_socket,
        client_addr));
    this->_socketPollList.addSocket(client_socket, POLLIN);
    printf("%s:%d connected fd: %d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port), client_socket);
    write(client_socket, "220 Service ready for new user.\r\n", 34);
}
