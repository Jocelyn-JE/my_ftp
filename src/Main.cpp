/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Main
*/

#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "RunServer.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    ftp::Parser parser(argc, argv);

    try {
        parser.parseArgs();
    } catch(const ftp::Parser::ParsingError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser.getUsage() << std::endl;
        return 84;
    }
    return ftp::runServer(atoi(argv[1]), argv[2]);
}
