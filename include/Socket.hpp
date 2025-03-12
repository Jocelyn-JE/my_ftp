/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Socket
*/

#ifndef SOCKET_HPP_
    #define SOCKET_HPP_

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
    struct sockaddr_in getAddress();
    void bindSocket(uint16_t port);
    void listenSocket(int backlog);
    void writeToSocket(std::string);
    std::string readFromSocket();
    void closeSocket();
    bool closesOnDestroy();
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

#endif  // SOCKET_HPP_
