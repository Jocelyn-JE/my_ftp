/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** PollFdList
*/

#ifndef POLLFDLIST_HPP_
    #define POLLFDLIST_HPP_

    #include <poll.h>
    #include <vector>

namespace ftp {
    class PollFdList : public std::vector<struct pollfd> {
        public:
            PollFdList() = delete;
            PollFdList(int serverSocket);
            ~PollFdList();
            void addSocket(int socket, short events);
            void removeSocket(int socket);
    };
}

#endif /* !POLLFDLIST_HPP_ */
