/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Parser
*/

#ifndef PARSER_HPP_
    #define PARSER_HPP_

    #include <exception>
    #include <string>
    #include <vector>

namespace ftp {
class Parser {
 public:
        Parser() = delete;
        Parser(int argc, char **argv);
        ~Parser();
        class ParsingError : public std::exception {
         public:
            explicit ParsingError(std::string message);
            ~ParsingError();
            const char *what() const noexcept override;
         private:
            std::string _message;
        };
        void parseArgs();
        std::string getUsage();
 private:
        std::vector<std::string> _args;
        int _argc;
};
}  // namespace ftp

#endif  // PARSER_HPP_
