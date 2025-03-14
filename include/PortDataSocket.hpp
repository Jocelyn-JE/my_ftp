/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Datasocket
*/

#ifndef PORTDATASOCKET_HPP_
    #define PORTDATASOCKET_HPP_
    #include <string>
    #include "Socket.hpp"
    #include "IDataSocket.hpp"

namespace ftp {
class PortDataSocket : private Socket, public IDataSocket {
 public:
    PortDataSocket() = delete;
    explicit PortDataSocket(std::string address);
    ~PortDataSocket() noexcept(false) override;
    void connectToClient() override {};
    void writeToClient(std::string data) override;
    std::string getIpStr() const override;
    std::string getPortStr() const override;
 protected:
 private:
};  // class PortDataSocket
}  // namespace ftp

#endif  // PORTDATASOCKET_HPP_
