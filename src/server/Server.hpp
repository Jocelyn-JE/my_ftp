/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#ifndef SRC_SERVER_SERVER_HPP_
    #define SRC_SERVER_SERVER_HPP_

    #include <memory>
    #include <vector>
    #include <string>
    #include "Socket.hpp"
    #include "PollFdList.hpp"
    #include "Client.hpp"
    #define LISTEN_BACKLOG 128

namespace ftp {
class Server {
 public:
    Server() = delete;
    Server(int port, std::string rootPath);
    ~Server();
    int pollSockets();
    void updateSockets();
    void handleConnection();
    void handleDisconnection(int socketIndex);
    bool isClosed();
 private:
    std::vector<std::unique_ptr<Client>> _clients;
    Socket _serverSocket;
    PollFdList _socketPollList;
    std::string _anonymousRootPath;
};
}  // namespace ftp

#endif  // SRC_SERVER_SERVER_HPP_
