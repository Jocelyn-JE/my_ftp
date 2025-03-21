/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Main
*/

#include <iostream>
#include "server/Server.hpp"
#include "parsing/Parser.hpp"
#include "server/RunServer.hpp"

int main(int argc, char **argv) {
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
