/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

namespace ftp {
    class Server {
        public:
            Server();
            ~Server();
        protected:
        private:
            int _socket;
    };
}

#endif /* !SERVER_HPP_ */
