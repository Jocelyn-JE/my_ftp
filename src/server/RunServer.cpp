/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** RunServer
*/

#include "RunServer.hpp"
#include "Server.hpp"
#include <iostream>

int ftp::runServer(int port, std::string rootPath)
{
    int poll_result = 0;

    try {
        ftp::Server server(port, rootPath);

        while (poll_result != -1) {
            poll_result = server.pollSockets();
            server.updateSockets();
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}