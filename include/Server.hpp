/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

    #include "Socket.hpp"
    #include "PollFdList.hpp"
    #define LISTEN_BACKLOG 128

namespace ftp {
    class Server {
        public:
            Server() = delete;
            Server(int port, std::string rootPath);
            ~Server();
            int pollSockets();
            void updateSockets();
        protected:
        private:
            Socket _socket;
            PollFdList _socketList;
    };
}

#endif /* !SERVER_HPP_ */
