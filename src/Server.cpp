//
// Created by Алексей Подоплелов on 16.04.2026.
//

#include <Server.h>

Server::Server(boost::asio::io_context& io_context, unsigned Port) :
    udpSocket_(io_context, udpNamespace::endpoint(udpNamespace::v4(), Port)),
    tcpAcceptor_(io_context, tcpNamespace::endpoint(tcpNamespace::v4(), Port))
{
    this->listen_tcp();
}

void Server::listen_tcp()
{
    tcpAcceptor_.async_accept
    (
        [this](boost::system::error_code ec, tcpNamespace::socket socket)
        {
            if ( !ec ) {
                std::make_shared<BoostClientSession>(std::move(socket))->start_session();
            }
            this->listen_tcp();
        }
    );
}