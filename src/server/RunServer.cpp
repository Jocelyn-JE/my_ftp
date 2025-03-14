/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** RunServer
*/

#include <signal.h>
#include <iostream>
#include <string>
#include "server/RunServer.hpp"
#include "server/Server.hpp"
#include "parsing/DirectoryUtility.hpp"

volatile sig_atomic_t stopFlag = 0;

static void handler(int signum) {
    stopFlag = signum;
}

int ftp::runServer(int port, std::string rootPath) {
    int poll_result = 0;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    try {
        ftp::Server server(port, ftp::DirectoryUtility::removeTrailingSlash(
            &rootPath));

        while (poll_result != -1) {
            if (stopFlag)
                break;
            poll_result = server.pollSockets();
            server.updateSockets();
        }
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
