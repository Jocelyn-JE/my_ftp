/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Server
*/

#include "Server.hpp"
#include <sys/socket.h>

ftp::Server::Server()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
}

ftp::Server::~Server()
{
}
