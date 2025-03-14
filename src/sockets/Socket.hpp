/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Socket
*/

#ifndef SRC_SOCKETS_SOCKET_HPP_
    #define SRC_SOCKETS_SOCKET_HPP_

    #include <netinet/in.h>
    #include <string>
    #include <exception>

namespace ftp {
class Socket {
 public:
    Socket() = delete;
    Socket(int fd, struct sockaddr_in address);
    Socket(int domain, int type, int protocol);
    ~Socket() noexcept(false);
    int getSocketFd() const;
    struct sockaddr_in getAddress() const;
    void bindSocket(uint16_t port);
    void listenSocket(int backlog);
    void connectSocket(const struct sockaddr *address, socklen_t addressLen);
    void writeToSocket(std::string);
    std::string readFromSocket();
    void closeSocket();
    bool closesOnDestroy();
    void setCloseOnDestroy(bool closeOnDestroy);
    class SocketError : public std::exception {
     public:
        explicit SocketError(std::string message);
        ~SocketError();
        const char *what() const noexcept override;
     private:
        std::string _message;
    };

 private:
    bool _closeSocketOnDestruction;
    int _socketFd;
    struct sockaddr_in _address;
};
}  // namespace ftp

#endif  // SRC_SOCKETS_SOCKET_HPP_
