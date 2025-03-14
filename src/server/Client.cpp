/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "../../include/Client.hpp"
#include "../../include/DirectoryUtility.hpp"
#include "../../include/PasvDataSocket.hpp"
#include "../../include/PortDataSocket.hpp"


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

static bool isValidPortArgument(const std::string &portArg) {
    std::stringstream ss(portArg);
    std::vector<int> parts;
    std::string token;
    int num;

    // Split the input by commas
    while (std::getline(ss, token, ',')) {
        try {
            num = std::stoi(token);
            parts.push_back(num);
        } catch (...) {
            return false;  // Not a valid number
        }
    }
    // Must have exactly 6 parts (h1, h2, h3, h4, p1, p2)
    if (parts.size() != 6)
        return false;
    // Validate IP address (h1-h4) must be in range [0,255]
    for (int part : parts) {
        if (part < 0 || part > 255)
            return false;
    }
    // Validate port number (p1, p2)
    num = (parts[4] * 256) + parts[5];
    if (num < 1 || num > 65535)
        return false;
    return true;
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
    if (client->_currentPath == "")
        return "550 Requested action not taken.";
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
        return "550 Requested file action not taken. File unavailable";
    }
    if (remove(path.c_str()) == -1) {
        std::cout << "Failed to delete file: " << path << std::endl;
        return "550 Requested file action not taken. File unavailable";
    }
    return "250 Requested file action okay, completed.";
}

// FTP Commands that use the data socket functions ----------------------------

// This function creates a new socket on an available port and listens for
// incoming connections. It then sends the IP and port to the client.
// The socket must be closed after any data transfer is complete.
static std::string doPasv(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "PASV")
        return "501 Syntax error in parameters or arguments.";
    if (client->_dataSocket == nullptr)
        client->_dataSocket = std::make_unique<ftp::PasvDataSocket>();
    return "227 Entering Passive Mode (" + client->_dataSocket->getIpStr()
        + "," + client->_dataSocket->getPortStr() + ").";
}

// This function creates a new socket and connects to the given IP and port.
// The socket must be closed after any data transfer is complete.
static std::string doPort(std::string commandLine, ftp::Client *client) {
    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine.size() < 5 + 11 || commandLine.substr(0, 5) != "PORT ")
        return "501 Syntax error in parameters or arguments.";
    std::string address = commandLine.substr(5);
    if (!isValidPortArgument(address))
        return "501 Syntax error in parameters or arguments.";
    if (client->_dataSocket == nullptr)
        client->_dataSocket = std::make_unique<ftp::PortDataSocket>(address);
    return "200 Command okay.";
}

// Not finished yet
static std::string doList(std::string commandLine, ftp::Client *client) {
    std::string path;

    if (!client->isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "LIST" && (commandLine.size() < 6 ||
        commandLine.substr(0, 5) != "LIST "))
        return "501 Syntax error in parameters or arguments.";
    try {
        path = commandLine.substr(0, 5) == "LIST " ? ftp::DirectoryUtility::
            resolvePath(client->getRootPath(), client->getFullPath(),
            commandLine.substr(5)) : client->getFullPath();
    } catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
        return "450 Requested file action not taken.";
    }
    int pid = fork();
    if (pid == -1) {
        std::cout << "Failed to fork process" << std::endl;
        return "450 Requested file action not taken.";
    }
    if (pid == 0) {
        if (client->_dataSocket == nullptr)
            return "425 Can't open data connection.";
        std::cout << "List command child connecting to client" << std::endl;
        client->_dataSocket->connectToClient();
        std::cout << "List command child writing to dataSocket" << std::endl;
        client->_dataSocket->writeToClient("TEST");
        std::cout << "List command child writing to controlSocket" << std::endl;
        client->_controlSocket.writeToSocket("226 Transfer complete; "
            "Closing data connection.");
        std::cout << "List command child exiting" << std::endl;
        std::exit(0);
    } else {
        std::cout << "List command parent destroying _dataSocket" << std::endl;
        client->_dataSocket.reset(nullptr);
    }
    return "150 File status okay; about to open data connection.";
}

// Not finished yet
static std::string doRetr(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "502 Not implemented.";
}

// Not finished yet
static std::string doStor(std::string commandLine, ftp::Client *client) {
    (void)commandLine;
    (void)client;
    return "502 Not implemented.";
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
    if (_commands.find(name) == _commands.end() || commandLine.length() > 512)
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
