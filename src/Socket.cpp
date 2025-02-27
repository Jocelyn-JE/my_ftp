/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Socket
*/

#include "Socket.hpp"
#include <string.h>
#include <unistd.h>
#include <iostream>

ftp::Socket::Socket(int fd, struct sockaddr_in address) :
    _closeSocketOnDestruction(false)
{
    _socketFd = fd;
    _address = address;
    std::cout << "Fetched new socket on fd: " << _socketFd << std::endl;
}

ftp::Socket::Socket(int domain, int type, int protocol) :
    _closeSocketOnDestruction(true)
{
    _socketFd = socket(domain, type, protocol);
    if (_socketFd == -1)
        throw ftp::Socket::SocketError("Socket creation failed: " +
            std::string(strerror(errno)));
    std::cout << "Created socket on fd: " << _socketFd << std::endl;
}

ftp::Socket::~Socket() noexcept(false)
{
    if (_socketFd >= 0 && _closeSocketOnDestruction) {
        if (close(_socketFd) == -1)
            throw ftp::Socket::SocketError("Failed to close socket: " +
                std::string(strerror(errno)));
        std::cout << "Closed socket on fd: " << _socketFd << std::endl;
    }
    std::cout << "Removed socket on fd " << _socketFd << " from server" <<
        std::endl;
}

bool ftp::Socket::closesOnDestroy()
{
    return _closeSocketOnDestruction;
}

void ftp::Socket::closeSocket()
{
    if (close(_socketFd) == -1)
        throw ftp::Socket::SocketError("Failed to close socket: " +
            std::string(strerror(errno)));
    std::cout << "Closed socket on fd: " << _socketFd << std::endl;
    _closeSocketOnDestruction = false;
}

int ftp::Socket::getSocketFd() const
{
    return _socketFd;
}

void ftp::Socket::bindSocket(uint16_t port)
{
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(_socketFd, (const struct sockaddr *) &_address,
        sizeof(_address)) == -1)
        throw ftp::Socket::SocketError("Bind failed: " +
            std::string(strerror(errno)));
}

void ftp::Socket::listenSocket(int backlog)
{
    if (listen(_socketFd, backlog))
        throw ftp::Socket::SocketError("Listen failed: " +
            std::string(strerror(errno)));
}

ftp::Socket::SocketError::SocketError(std::string message)
{
    _message = message;
}

ftp::Socket::SocketError::~SocketError()
{
}

const char *ftp::Socket::SocketError::what() const noexcept
{
    return _message.c_str();
}

void ftp::Socket::writeToSocket(std::string str)
{
    std::string crlfEndedString = str + "\r\n";

    if (str == "")
        return;
    if (write(this->_socketFd, crlfEndedString.c_str(),
        crlfEndedString.length()) == -1) {
        throw ftp::Socket::SocketError("Write on fd " + _socketFd +
            (" failed: " + std::string(strerror(errno))));
    }
}

std::string ftp::Socket::readFromSocket()
{
    char buffer[BUFSIZ];
    int bytes_read = read(_socketFd, buffer, sizeof(buffer));

    buffer[bytes_read] = '\0';
    if (bytes_read < 0) {
        throw ftp::Socket::SocketError("Read on fd " + _socketFd +
            (" failed: " + std::string(strerror(errno))));
    }
    return buffer;
}
