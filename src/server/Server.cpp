/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "../../include/Server.hpp"

ftp::Server::Server(int port, std::string rootPath) :
    _serverSocket(AF_INET, SOCK_STREAM, 0),
    _socketPollList(_serverSocket.getSocketFd()),
    _anonymousRootPath(rootPath) {
    _serverSocket.bindSocket(port);
    _serverSocket.listenSocket(LISTEN_BACKLOG);
    std::cout << "Server started on port " << port << std::endl;
    std::cout << "Root path: " << rootPath << std::endl;
}

ftp::Server::~Server() {
}

int ftp::Server::pollSockets() {
    int result = poll(_socketPollList.data(), _clients.size() + 1, -1);
    if (result == -1)
        throw ftp::Socket::SocketError("Poll failed: " +
            std::string(strerror(errno)));
    return result;
}

bool ftp::Server::isClosed() {
    return !_serverSocket.closesOnDestroy();
}

// Iterate through all clients with _clients.size() (+ 1 for the server socket)
// and update/execute depending on values read from the sockets.
// Server socket is _serverSocket polling is _socketPollList[0]
void ftp::Server::updateSockets() {
    std::string socketStr;
    std::string buffer;

    for (std::size_t i = 0; i < _socketPollList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN) {
            if (i == 0) {
                handleConnection();
            } else {
                buffer = _clients[i - 1]->_controlSocket.readFromSocket();
                _clients[i - 1]->handleCommand(buffer);
                if (buffer == "" || buffer == "QUIT\r\n")
                    handleDisconnection(i);
            }
        }
    }
}

// This function does not close the given socket, it only removes it from the
// socket list and poll list
void ftp::Server::handleDisconnection(int socketIndex) {
    _clients.erase(_clients.begin() + (socketIndex - 1));
    _socketPollList.removeSocket(_socketPollList[socketIndex].fd);
}

void ftp::Server::handleConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(_serverSocket.getSocketFd(),
        (struct sockaddr *) &client_addr, &client_addr_size);

    this->_clients.push_back(std::make_unique<Client>(client_socket,
        client_addr, this->_anonymousRootPath));
    this->_socketPollList.addSocket(client_socket, POLLIN);
    std::cout << inet_ntoa(client_addr.sin_addr) << ":"
        << ntohs(client_addr.sin_port) << " connected fd: " << client_socket
        << std::endl;
    this->_clients.back()->_controlSocket.writeToSocket(
        "220 Service ready for new user.");
}
