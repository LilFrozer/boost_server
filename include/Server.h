//
// Created by Алексей Подоплелов on 16.04.2026.
//

#ifndef MIL_SERVER_H
#define MIL_SERVER_H

#include <ClientSession.h>

using udpNamespace = boost::asio::ip::udp;
using tcpNamespace = boost::asio::ip::tcp;

class Server : public std::enable_shared_from_this<Server>
{
private:
    udpNamespace::socket udpSocket_{nullptr};
    udpNamespace::endpoint udpEndpoint_;
    std::array<char, 1024> udpRecvBuf_{};
private:
    tcpNamespace::acceptor tcpAcceptor_;
public:
    void listen_tcp();
private:
    // void start_receive()
    // {
    //     m_socket.async_receive_from(
    //         boost::asio::buffer(m_recv_buffer),
    //         m_remote_endpoint,
    //         boost::bind(&Server::handle_receive, this,
    //                     boost::asio::placeholders::error,
    //                     boost::asio::placeholders::bytes_transferred));
    // }
    // void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
    // {
    //     if (!error) {
    //         std::string received(m_recv_buffer.data(), bytes_transferred);
    //         std::cout << "Received from " << m_remote_endpoint.address().to_string()
    //                   << ":" << m_remote_endpoint.port() << " -> " << received << std::endl;
    //
    //         // Эхо-ответ - отправляем то же самое обратно
    //         std::string response = "Echo: " + received;
    //         m_socket.async_send_to(
    //             boost::asio::buffer(response),
    //             m_remote_endpoint,
    //             boost::bind(&Server::handle_send, this,
    //                         boost::asio::placeholders::error,
    //                         boost::asio::placeholders::bytes_transferred));
    //     } else {
    //         std::cerr << "Receive error: " << error.message() << std::endl;
    //     }
    //
    //     // Продолжаем слушать следующие сообщения
    //     this->start_receive();
    // }
    // void handle_send(const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
    // {
    //     if (error) {
    //         std::cerr << "Send error: " << error.message() << std::endl;
    //     }
    // }
public:
    Server(boost::asio::io_context& io_context, const std::string &addr, const uint16_t &port);
    ~Server();
};


#endif //MIL_SERVER_H
