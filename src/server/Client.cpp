/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Client
*/

#include "Client.hpp"

static std::string doNoop(std::string commandLine, ftp::Client &client)
{
    (void)client;
    if (commandLine != "NOOP")
        return "501 Syntax error in parameters or arguments.";
    return "200 Command okay.";
}

static std::string doQuit(std::string commandLine, ftp::Client &client)
{
    (void)commandLine;
    if (commandLine != "QUIT")
        return "501 Syntax error in parameters or arguments.";
    printf("Disconnected client %d\n", client._socket.getSocketFd());
    return "221 Service closing control connection.";
}

static std::string clientDisconnect(std::string commandLine,
    ftp::Client &client)
{
    (void)commandLine;
    printf("Client %d disconnected\n", client._socket.getSocketFd());
    return "";
}

static std::string doUser(std::string commandLine, ftp::Client &client)
{
    std::string tempUser;

    if (commandLine.size() < 6 || commandLine.substr(0, 5) != "USER ")
        return "501 Syntax error in parameters or arguments.";
    tempUser = commandLine.substr(5);
    printf("User: %s\n", tempUser.c_str());
    client._username = tempUser;
    return "331 User name okay, need password.";
}

static std::string checkLogin(ftp::Client &client)
{
    if (client.isLoggedIn()) {
        printf("User failed to log in\n");
        return "230 User logged in, proceed.";
    }
    printf("User successful log in\n");
    return "530 Not logged in.";
}

static std::string doPass(std::string commandLine, ftp::Client &client)
{
    std::string tempPass;

    if (client._username == "")
        return "332 Need account for login.";
    if (commandLine.size() < 5 || commandLine.substr(0, 5) != "PASS ")
        return "501 Syntax error in parameters or arguments.";
    if (client._username == "")
        return "503 Bad sequence of commands.";
    tempPass = commandLine.substr(5);
    printf("Pass: %s\n", tempPass.c_str());
    client._password = tempPass;
    return checkLogin(client);
}

static std::string doPwd(std::string commandLine, ftp::Client &client)
{
    if (!client.isLoggedIn())
        return "530 Not logged in.";
    if (commandLine != "PWD")
        return "501 Syntax error in parameters or arguments.";
    return "257 \"" + client.getFullPath() + "\" is the current directory.";
}

//-----------------------------------------------------------------------------

ftp::Client::Client(int fd, struct sockaddr_in address, std::string rootPath)
    : _socket(fd, address), _username(""), _password(""),
    _currentPath("/"), _rootPath(rootPath)
{
    _commands["NOOP"] = doNoop;
    _commands["QUIT"] = doQuit;
    _commands["USER"] = doUser;
    _commands["PASS"] = doPass;
    _commands["PWD"] = doPwd;
}

ftp::Client::~Client()
{
    printf("Destroying client\n");
}

// Function to extract the command from the command line and remove CRLF
static std::string getCommand(const std::string &commandLine)
{
    size_t spacePos = commandLine.find(' ');
    std::string command = (spacePos == std::string::npos) ? commandLine
        : commandLine.substr(0, spacePos);

    if (commandLine[0] == ' ')
        return commandLine;
    if (command.size() >= 2 && command.substr(command.size() - 2) == "\r\n")
        command = command.substr(0, command.size() - 2);
    return command;
}

void ftp::Client::handleCommand(std::string commandLine)
{
    std::string name = getCommand(commandLine);

    if (commandLine == "") {
        clientDisconnect(commandLine, *this);
        return;
    }
    if (_commands.find(name) == _commands.end()) {
        _socket.writeToSocket("500 Syntax error, command unrecognized.");
        return;
    }
    _socket.writeToSocket(_commands[name](
        commandLine.substr(0, commandLine.size() - 2), *this));
    if (commandLine == "QUIT\r\n")
        _socket.closeSocket();
}

bool ftp::Client::isLoggedIn() const
{
    if (_username == "Anonymous" && _password == "")
        return true;
    return false;
}

std::string ftp::Client::getFullPath()
{
    return _rootPath + _currentPath;
}
