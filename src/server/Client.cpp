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
    if (commandLine.size() < 6 || commandLine.substr(0, 5) != "USER ")
        return "501 Syntax error in parameters or arguments.";
    client._username = commandLine.substr(5, commandLine.size() - 7);
    return "331 User name okay, need password.";
}

static std::string doPass(std::string commandLine, ftp::Client &client)
{
    if (commandLine.size() < 5 || commandLine.substr(0, 5) != "PASS ")
        return "501 Syntax error in parameters or arguments.";
    if (client._username == "") {
        return "503 Bad sequence of commands.";
    }
    client._password = commandLine.substr(5, commandLine.size() - 7);
    return "230 User logged in, proceed.";
}

//-----------------------------------------------------------------------------

ftp::Client::Client(int fd, struct sockaddr_in address) : _socket(fd, address)
{
    _commands[""] = clientDisconnect;
    _commands["NOOP"] = doNoop;
    _commands["QUIT"] = doQuit;
    _commands["USER"] = doUser;
    _commands["PASS"] = doPass;
}

ftp::Client::~Client()
{
    printf("Destroying client\n");
}

// Function to trim leading and trailing spaces
static std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to extract the command from the command line and remove CRLF
static std::string getCommand(const std::string &commandLine)
{
    std::string trimmedCommandLine = trim(commandLine);
    size_t spacePos = trimmedCommandLine.find(' ');
    std::string command = (spacePos == std::string::npos) ? trimmedCommandLine
        : trimmedCommandLine.substr(0, spacePos);

    if (command.size() >= 2 && command.substr(command.size() - 2) == "\r\n")
        command = command.substr(0, command.size() - 2);
    return command;
}

void ftp::Client::handleCommand(std::string commandLine)
{
    std::string name = getCommand(commandLine);

    if (_commands.find(name) == _commands.end()) {
        _socket.writeToSocket("500 Syntax error, command unrecognized.");
        return;
    }
    _socket.writeToSocket(_commands[name](
        commandLine.substr(0, commandLine.size() - 2), *this));
    if (commandLine == "QUIT\r\n")
        _socket.closeSocket();
}
