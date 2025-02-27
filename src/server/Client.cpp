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
    if (commandLine != "NOOP\r\n")
        return "501 Syntax error in parameters or arguments.";
    return "200 Command okay.";
}

static std::string doQuit(std::string commandLine, ftp::Client &client)
{
    (void)commandLine;
    if (commandLine != "QUIT\r\n")
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

//-----------------------------------------------------------------------------

ftp::Client::Client(int fd, struct sockaddr_in address) : _socket(fd, address)
{
    _commands[""] = clientDisconnect;
    _commands["NOOP"] = doNoop;
    _commands["QUIT"] = doQuit;
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

void ftp::Client::run(std::string commandLine)
{
    std::string name = getCommand(commandLine);

    if (_commands.find(name) == _commands.end()) {
        _socket.writeToSocket("500 Syntax error, command unrecognized.");
        return;
    }
    _socket.writeToSocket(_commands[name](commandLine, *this));
    if (commandLine == "QUIT\r\n")
        _socket.closeSocket();
}
