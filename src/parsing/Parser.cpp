/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Parser
*/

#include <iostream>
#include <string>
#include "parsing/Parser.hpp"
#include "parsing/DirectoryUtility.hpp"

ftp::Parser::Parser(int argc, char **argv) : _args(argv, argv + argc),
    _argc(argc) {
}

ftp::Parser::~Parser() {
}

static bool isNumber(std::string str) {
    char *p = 0;

    strtol(str.c_str(), &p, 10);
    return *p == 0;
}

std::string ftp::Parser::getUsage() {
    return R"(USAGE: ./my_ftp port path
    port is the port number on which the server socket listens
    path is the path to the home directory for the Anonymous user)";
}

void ftp::Parser::parseArgs() {
    if (_argc == 2 && _args[1] == "-help") {
        std::cout << getUsage() << std::endl;
        exit(0);
    }
    if (_argc < 3)
        throw ftp::Parser::ParsingError("Not enough args");
    if (!isNumber(_args[1]))
        throw ftp::Parser::ParsingError("Port must be a number");
    if (!ftp::DirectoryUtility::dirExists(_args[2]))
        throw ftp::Parser::ParsingError("Path does not exist");
}

ftp::Parser::ParsingError::ParsingError(std::string message) {
    _message = message;
}

ftp::Parser::ParsingError::~ParsingError() {
}

const char *ftp::Parser::ParsingError::what() const noexcept {
    return _message.c_str();
}
