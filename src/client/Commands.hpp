/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Commands
*/

#ifndef SRC_CLIENT_COMMANDS_HPP_
    #define SRC_CLIENT_COMMANDS_HPP_
    #include <string>
    #include "client/Client.hpp"

namespace ftp {
class Commands {
 public:
    static std::string doUser(std::string commandLine, Client &client);
    static std::string doPass(std::string commandLine, Client &client);
    static std::string doCwd(std::string commandLine, Client &client);
    static std::string doCdup(std::string commandLine, Client &client);
    static std::string doQuit(std::string commandLine, Client &client);
    static std::string doDelete(std::string commandLine, Client &client);
    static std::string doPwd(std::string commandLine, Client &client);
    static std::string doPasv(std::string commandLine, Client &client);
    static std::string doPort(std::string commandLine, Client &client);
    static std::string doNoop(std::string commandLine, Client &client);
    static std::string doHelp(std::string commandLine, Client &client);
    static std::string doRetr(std::string commandLine, Client &client);
    static std::string doList(std::string commandLine, Client &client);
    static std::string doStor(std::string commandLine, Client &client);
};
}  // namespace ftp

#endif  // SRC_CLIENT_COMMANDS_HPP_
