/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** Datasocket
*/

#ifndef SRC_SERVER_PASVDATASOCKET_HPP_
    #define SRC_SERVER_PASVDATASOCKET_HPP_
    #include <string>
    #include <memory>
    #include "Socket.hpp"
    #include "IDataSocket.hpp"

namespace ftp {
class PasvDataSocket : private Socket, public IDataSocket {
 public:
    PasvDataSocket();
    ~PasvDataSocket() noexcept(false) override;
    void connectToClient() override;
    void writeToClient(std::string data) override;
    std::string getIpStr() const override;
    std::string getPortStr() const override;
 protected:
 private:
    void acceptConnection();
    std::unique_ptr<Socket> _connectedClientSocket;
};  // class PasvDataSocket
}  // namespace ftp

#endif  // SRC_SERVER_PASVDATASOCKET_HPP_
