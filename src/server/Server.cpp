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
    int result = poll(_socketPollList.data(), _socketList.size(), -1);
    if (result == -1)
        throw ftp::Socket::SocketError("Poll failed: " +
            std::string(strerror(errno)));
    return result;
}

bool ftp::Server::isClosed()
{
    return !_socketList[0]->closesOnDestroy();
}

void ftp::Server::updateSockets()
{
    std::string socketStr;

    for (std::size_t i = 0; i < _socketList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN) {
            if (_socketPollList[i].fd == _socketList[0]->getSocketFd()) {
                handleConnection();
            } else {
                std::string buffer = "";

                buffer = _socketList[i]->readFromSocket();
                if (buffer == "") {
                    handleDisconnection(i);
                    printf("Client %d disconnected\n", _socketPollList[i].fd);
                } else {
                    if (buffer == "QUIT\r\n") {
                        dprintf(_socketPollList[i].fd, "221 Service closing control connection.\r\n");
                        _socketList[i]->closeSocket();
                        handleDisconnection(i);
                        printf("Disconnected client %d\n", _socketPollList[i].fd);
                    }
                }
            }
        }
    }
}

// This function does not close the given socket, it only removes it from the
// socket list and poll list
void ftp::Server::handleDisconnection(int socketIndex)
{
    _socketList.erase(_socketList.begin() + socketIndex);
    _socketPollList.removeSocket(_socketPollList[socketIndex].fd);
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
    this->_socketList.back()->writeToSocket("220 Service ready for new user.");
}
