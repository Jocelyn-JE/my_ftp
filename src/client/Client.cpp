/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#include <iostream>
#include <string>
#include "client/Client.hpp"
#include "client/Commands.hpp"

// Helper functions -----------------------------------------------------------

static void clientDisconnect(ftp::Client const &client) {
    std::cout << "Client " << client._controlSocket.getSocketFd()
        << " disconnected" << std::endl;
}

// Function to extract the command from the command line and remove CRLF
static std::string getCommand(const std::string &commandLine) {
    size_t spacePos = commandLine.find(' ');
    std::string command = (spacePos == std::string::npos) ? commandLine
        : commandLine.substr(0, spacePos);

    if (commandLine[0] == ' ')
        return commandLine;
    if (command.size() >= 2 && command.substr(command.size() - 2) == "\r\n")
        command = command.substr(0, command.size() - 2);
    return command;
}

// Client class member functions ----------------------------------------------

ftp::Client::Client(int fd, struct sockaddr_in address, std::string rootPath)
    : _controlSocket(fd, address), _dataSocket(nullptr), _username(""),
    _password("thisisaverybadwaytohandlepasswords"), _currentPath(""),
    _rootPath(rootPath) {
    _commands["USER"] = Commands::doUser;
    _commands["PASS"] = Commands::doPass;
    _commands["CWD"]  = Commands::doCwd;
    _commands["CDUP"] = Commands::doCdup;
    _commands["QUIT"] = Commands::doQuit;
    _commands["DELE"] = Commands::doDelete;
    _commands["PWD"]  = Commands::doPwd;
    _commands["PASV"] = Commands::doPasv;
    _commands["PORT"] = Commands::doPort;
    _commands["HELP"] = Commands::doHelp;
    _commands["NOOP"] = Commands::doNoop;
    _commands["RETR"] = Commands::doRetr;
    _commands["STOR"] = Commands::doStor;
    _commands["LIST"] = Commands::doList;
}

ftp::Client::~Client() {
    std::cout << "Destroying client" << std::endl;
}

void ftp::Client::handleCommand(std::string commandLine) {
    std::string name = getCommand(commandLine);

    if (commandLine == "")
        return clientDisconnect(*this);
    if (_commands.find(name) == _commands.end() || commandLine.length() > 512)
        return _controlSocket.writeToSocket(
            "500 Syntax error, command unrecognized.");
    _controlSocket.writeToSocket(
        _commands[name](commandLine.substr(0, commandLine.size() - 2), *this));
    if (commandLine == "QUIT\r\n")
        _controlSocket.closeSocket();
}

bool ftp::Client::isLoggedIn() const {
    if (_username == "Anonymous" && _password == "")
        return true;
    return false;
}

std::string ftp::Client::getFullPath() {
    return _rootPath + _currentPath;
}

std::string ftp::Client::getRootPath() {
    return _rootPath;
}
