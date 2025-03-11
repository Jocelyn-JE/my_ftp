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
    #include <cstdint>

namespace ftp {
class PollFdList : public std::vector<struct pollfd> {
 public:
    PollFdList() = delete;
    explicit PollFdList(int serverSocket);
    ~PollFdList();
    void addSocket(int socket, int16_t events);
    void removeSocket(int socket);
};
}  // namespace ftp

#endif  // POLLFDLIST_HPP_
