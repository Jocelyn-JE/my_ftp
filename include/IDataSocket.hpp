/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** IDataSocket
*/

#ifndef IDATASOCKET_HPP_
    #define IDATASOCKET_HPP_
    #include <string>

namespace ftp {
class IDataSocket {
 public:
    virtual ~IDataSocket() noexcept(false) = default;
    virtual void connectToClient() = 0;
    virtual void writeToClient(std::string data) = 0;
    virtual std::string getIpStr() const = 0;
    virtual std::string getPortStr() const = 0;
 private:
};
}  // namespace ftp

#endif  // IDATASOCKET_HPP_
