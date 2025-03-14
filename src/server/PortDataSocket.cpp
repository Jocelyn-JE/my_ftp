/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** PortDataSocket
*/

#include "server/PortDataSocket.hpp"
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <vector>
#include <sstream>

static std::string getIpString(const std::string &portArg) {
    std::stringstream ss(portArg);
    std::vector<int> parts;
    std::string token;
    int num;

    // Split the input by commas
    while (std::getline(ss, token, ',')) {
        try {
            num = std::stoi(token);
            parts.push_back(num);
        } catch (...) {
            return "";  // Not a valid number
        }
    }
    return std::to_string(parts[0]) + "." + std::to_string(parts[1]) + "." +
           std::to_string(parts[2]) + "." + std::to_string(parts[3]);
}

static int getPortInt(const std::string &portArg) {
    std::stringstream ss(portArg);
    std::vector<int> parts;
    std::string token;
    int num;

    // Split the input by commas
    while (std::getline(ss, token, ',')) {
        try {
            num = std::stoi(token);
            parts.push_back(num);
        } catch (...) {
            return -1;  // Not a valid number
        }
    }
    return (parts[4] * 256) + parts[5];
}

ftp::PortDataSocket::PortDataSocket(std::string address) :
Socket(AF_INET, SOCK_STREAM, 0) {
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(getPortInt(address));
    addr.sin_addr.s_addr = inet_addr(getIpString(address).c_str());
    this->connectSocket((struct sockaddr *) &addr, sizeof(addr));
    std::cout << inet_ntoa(addr.sin_addr) << ":" <<
        ntohs(addr.sin_port) << " connected to active data socket on "
        "fd: " << this->getSocketFd() << std::endl;
    this->setCloseOnDestroy(true);
}

ftp::PortDataSocket::~PortDataSocket() {
}

void ftp::PortDataSocket::writeToClient(std::string data) {
    this->writeToSocket(data);
}

std::string ftp::PortDataSocket::getIpStr() const {
    struct sockaddr_in addr = this->getAddress();
    uint32_t ip = ntohl(addr.sin_addr.s_addr);
    return std::to_string((ip >> 24) & 0xFF) + "," +
           std::to_string((ip >> 16) & 0xFF) + "," +
           std::to_string((ip >> 8) & 0xFF) + "," +
           std::to_string(ip & 0xFF);
}

std::string ftp::PortDataSocket::getPortStr() const {
    struct sockaddr_in addr = this->getAddress();
    uint16_t port = ntohs(addr.sin_port);
    return std::to_string((port >> 8) & 0xFF) + "," +
           std::to_string(port & 0xFF);
}
