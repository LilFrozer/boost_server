//
// Created by Алексей Подоплелов on 16.04.2026.
//

#include <Server.h>
#include <iostream>

/**
 * constructor
 */
Server::Server(boost::asio::io_context& io_context, const std::string &addr, const uint16_t &port) :
    udpSocket_(io_context, udpNamespace::endpoint(boost::asio::ip::make_address_v4(addr), port)),
    tcpAcceptor_(io_context, tcpNamespace::endpoint(boost::asio::ip::make_address_v4(addr), port))
{
    std::cout << "Server started" << std::endl;
    this->listen_tcp();
}

/**
 * close all
 */
Server::~Server() {
    boost::system::error_code ec;
    tcpAcceptor_.close(ec);
    udpSocket_.close(ec);
}

/**
 * accept incoming client with tcp
 */
void Server::listen_tcp()
{
    tcpAcceptor_.async_accept
    (
        [this](boost::system::error_code ec, tcpNamespace::socket socket)
        {
            if ( !ec ) {
                std::cout << "Client connected" << std::endl;
                auto cl = std::make_shared<BoostClientSession>(std::move(socket));
                cl->start_session();
            }
            this->listen_tcp();
        }
    );
}