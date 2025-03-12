/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#include <string>
#include <iostream>
#include "../../include/Client.hpp"
#include "../../include/DirectoryUtility.hpp"

static const char helpMessage[] = "Usage: \n"
"USER <SP> <username> <CRLF>   : Specify user for authentication\n"
"PASS <SP> <password> <CRLF>   : Specify password for authentication\n\n"
"CWD  <SP> <pathname> <CRLF>   : Change working directory\n"
"CDUP <CRLF>                   : Change working directory to parent directory\n"
"\n"
"QUIT <CRLF>                   : Disconnection\n\n"
"DELE <SP> <pathname> <CRLF>   : Delete file on the server\n\n"
"PWD  <CRLF>                   : Print working directory\n\n"
"PASV <CRLF>                   : Enable \"passive\" mode for data transfer\n"
"PORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data transfer\n\n"
"HELP [<SP> <string>] <CRLF>   : List available commands\n\n"
"NOOP <CRLF>                   : Do nothing\n\n"
"(the following are commands using data transfer )\n\n"
"RETR <SP> <pathname> <CRLF>   : Download file from server to client\n"
"STOR <SP> <pathname> <CRLF>   : Upload file from client to server\n"
"LIST [<SP> <pathname>] <CRLF> : List files in the current working directory";

// Helper functions -----------------------------------------------------------

static void clientDisconnect(ftp::Client *client) {
    std::cout << "Client " << client->_controlSocket.getSocketFd()
        << " disconnected" << std::endl;
}

static std::string checkLogin(ftp::Client *client) {
    if (client->isLoggedIn()) {
        std::cout << "User successful log in" << std::endl;
        return "230 User logged in, proceed.";
    }
    std::cout << "User failed to log in" << std::endl;
    return "530 Not logged in.";
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

// FTP Command functions ------------------------------------------------------

static std::string doNoop(std::string commandLine, ftp::Client *client) {
    (void)client;
    if (commandLine != "NOOP")
        return "501 Syntax error in parameters or arguments.";
    return "200 Command okay.";
}

static std::string doQuit(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    if (commandLine != "QUIT")
        return "501 Syntax error in parameters or arguments.";
    std::cout << "Disconnected client " << client->_controlSocket.getSocketFd()
        << std::endl;
    return "221 Service closing control connection.";
}

static std::string doUser(std::string commandLine, ftp::Client *client) {
    std::string tempUser;

    if (commandLine.size() < 6 || commandLine.substr(0, 5) != "USER ")
        return "501 Syntax error in parameters or arguments.";
    tempUser = commandLine.substr(5);
    std::cout << "User: " << tempUser << std::endl;
    client->_username = tempUser;
    return "331 User name okay, need password.";
}

static std::string doPass(std::string commandLine, ftp::Client *client) {
    std::string tempPass;

    if (client->_username == "")
        return "332 Need account for login.";
    if (commandLine.size() < 5 || commandLine.substr(0, 5) != "PASS ")
        return "501 Syntax error in parameters or arguments.";
    if (client->_username == "")
        return "503 Bad sequence of commands.";
    tempPass = commandLine.substr(5);
    std::cout << "Pass: " << tempPass << std::endl;
    client->_password = tempPass;
    return checkLogin(client);
}

static std::string doPwd(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "PWD")
        return "501 Syntax error in parameters or arguments.";
    return "257 \"" + client->getFullPath() + "\" is the current directory.";
}

static std::string doCdup(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "CDUP")
        return "501 Syntax error in parameters or arguments.";
    client->_currentPath = ftp::DirectoryUtility::getParentDirectory(
        client->_currentPath);
    return "200 Command okay.";
}

static std::string doHelp(std::string commandLine, ftp::Client *client) {
    (void)client;
    if (commandLine != "HELP")
        return "501 Syntax error in parameters or arguments.";
    return "214 " + std::string(helpMessage) + ".";
}

static std::string doCwd(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine.size() < 5 || commandLine.substr(0, 4) != "CWD ")
        return "501 Syntax error in parameters or arguments.";
    std::string path = commandLine.substr(4);
    try {
        client->_currentPath = ftp::DirectoryUtility::resolveCanonicalPath(
            client->getRootPath(), client->getFullPath(), path);
    } catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
        return "550 Requested action not taken.";
    }
    return "250 Requested file action okay, completed.";
}

static std::string doDelete(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine.size() < 6 || commandLine.substr(0, 5) != "DELE ")
        return "501 Syntax error in parameters or arguments.";
    std::string path = commandLine.substr(5);
    try {
        path = ftp::DirectoryUtility::resolvePath(client->getRootPath(),
            client->getFullPath(), path);
        if (!ftp::DirectoryUtility::fileExists(path))
            throw std::runtime_error(path + " No such file.");
    } catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
        return "450 Requested file action not taken.";
    }
    if (remove(path.c_str()) == -1) {
        std::cout << "Failed to delete file: " << path << std::endl;
        return "450 Requested file action not taken.";
    }
    return "250 Requested file action okay, completed.";
}

// FTP Commands that use the data socket functions ----------------------------

static std::string doPasv(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "501 Not implemented.";
}

// Not finished yet
static std::string doPort(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "501 Not implemented.";
}

// Not finished yet
static std::string doList(std::string commandLine, ftp::Client *client) {
    bool pathProvided = false;
    std::string path;

    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "LIST" && (commandLine.size() < 6 ||
        commandLine.substr(0, 5) != "LIST "))
        return "501 Syntax error in parameters or arguments.";
    if (commandLine.substr(0, 5) == "LIST ")
        pathProvided = true;
    try {
        path = pathProvided ? ftp::DirectoryUtility::resolvePath(
            client->getRootPath(), client->getFullPath(), commandLine.substr(5))
            : client->getFullPath();
    } catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
        return "450 Requested file action not taken.";
    }
    // Implement data transfer
    return "501 " + path;
}

// Not finished yet
static std::string doRetr(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "501 Not implemented.";
}

// Not finished yet
static std::string doStor(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "501 Not implemented.";
}

// Client class member functions ----------------------------------------------

ftp::Client::Client(int fd, struct sockaddr_in address, std::string rootPath)
    : _controlSocket(fd, address), _dataSocket(nullptr), _username(""),
    _password(""), _currentPath(""), _rootPath(rootPath) {
    _commands["USER"] = doUser;
    _commands["PASS"] = doPass;
    _commands["CWD"]  = doCwd;
    _commands["CDUP"] = doCdup;
    _commands["QUIT"] = doQuit;
    _commands["DELE"] = doDelete;
    _commands["PWD"]  = doPwd;
    _commands["PASV"] = doPasv;
    _commands["PORT"] = doPort;
    _commands["HELP"] = doHelp;
    _commands["NOOP"] = doNoop;
    _commands["RETR"] = doRetr;
    _commands["STOR"] = doStor;
    _commands["LIST"] = doList;
}

ftp::Client::~Client() {
    std::cout << "Destroying client" << std::endl;
}

void ftp::Client::handleCommand(std::string commandLine) {
    std::string name = getCommand(commandLine);

    if (commandLine == "")
        return clientDisconnect(this);
    if (_commands.find(name) == _commands.end())
        return _controlSocket.writeToSocket(
            "500 Syntax error, command unrecognized.");
    _controlSocket.writeToSocket(
        _commands[name](commandLine.substr(0, commandLine.size() - 2), this));
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
