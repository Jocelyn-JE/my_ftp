/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** PasvDataSocket
*/

#include "sockets/PasvDataSocket.hpp"
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <stdexcept>
#include <memory>
#include <iostream>

ftp::PasvDataSocket::PasvDataSocket() : Socket(AF_INET, SOCK_STREAM, 0) {
    this->bindSocket(0);
    this->listenSocket(1);
    this->setCloseOnDestroy(true);
}

ftp::PasvDataSocket::~PasvDataSocket() {
}

std::string ftp::PasvDataSocket::getIpStr() const {
    struct sockaddr_in addr = this->getAddress();
    uint32_t ip = ntohl(addr.sin_addr.s_addr);
    return std::to_string((ip >> 24) & 0xFF) + "," +
           std::to_string((ip >> 16) & 0xFF) + "," +
           std::to_string((ip >> 8) & 0xFF) + "," +
           std::to_string(ip & 0xFF);
}

std::string ftp::PasvDataSocket::getPortStr() const {
    struct sockaddr_in addr = this->getAddress();
    uint16_t port = ntohs(addr.sin_port);
    return std::to_string((port >> 8) & 0xFF) + "," +
           std::to_string(port & 0xFF);
}

void ftp::PasvDataSocket::connectToClient() {
    pollfd fds[1] = {{this->getSocketFd(), POLLIN, 0}};
    int result = poll(fds, 1, -1);
    if (result == -1)
        throw ftp::Socket::SocketError("Poll failed: " +
            std::string(strerror(errno)));
    if (fds[0].revents & POLLIN)
        this->acceptConnection();
}

void ftp::PasvDataSocket::acceptConnection() {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = accept(this->getSocketFd(), (struct sockaddr *) &clientAddr,
        &addrLen);
    if (clientFd == -1)
        throw ftp::Socket::SocketError("Failed to accept connection");
    this->_connectedClientSocket = std::make_unique<Socket>(clientFd,
        clientAddr);
    std::cout << inet_ntoa(clientAddr.sin_addr) << ":" <<
        ntohs(clientAddr.sin_port) << " connected to passive data socket on "
        "fd: " << clientFd << std::endl;
    this->_connectedClientSocket->setCloseOnDestroy(true);
}

void ftp::PasvDataSocket::writeToClient(std::string data) {
    this->_connectedClientSocket->writeToSocket(data);
}
