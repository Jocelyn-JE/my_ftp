/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Main
*/

#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    ftp::Server server;
    ftp::Parser parser(argc, argv);

    try {
        parser.parseArgs();
    } catch(const ftp::Parser::ParsingError &e) {
        std::cerr << e.what() << '\n';
        std::cerr << parser.getUsage() << '\n';
        return 84;
    }
    return 0;
}
