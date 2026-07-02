#include "Server.h"

IServer::IServer() : imitator_far_(std::make_unique<ImitatorFar>()) {
    if (imitator_far_->Start() != SocketError::SUCCESS) {
        std::cerr << "Code error:" << std::endl;    // ????
    } else {
        std::cout << "imitator_far_->Listen()..." << std::endl;
    }
}

BoostServer::BoostServer
(
    boost::asio::io_context& io_context,
    const std::string &addr,
    const uint16_t &port
) : IServer(),
    udp_socker_(io_context, boost_udp::endpoint(boost::asio::ip::make_address_v4(addr), port)),
    tcp_acceptor_(io_context, boost_tcp::endpoint(boost::asio::ip::make_address_v4(addr), port))
{
}

BoostServer::~BoostServer() {
    boost::system::error_code ec;
    tcp_acceptor_.close(ec);
    udp_socker_.close(ec);
}

void BoostServer::ListenTcp() {
    std::cout << "BoostServer::ListenTcp()..." << std::endl;
    tcp_acceptor_.async_accept (
        [this](boost::system::error_code ec, boost_tcp::socket socket)
        {
            if ( !ec ) {
                std::cout << "Client connected" << std::endl;
                tcp_data::FirstData fd{};
                boost::system::error_code ec;
                auto remote_ep = socket.remote_endpoint(ec);
                if (!ec) {
                    std::string ip = remote_ep.address().to_string();
                    fd.client_addr = ip;
                    u16 port = remote_ep.port();
                    fd.client_port = port;
                    std::cout << "Remote: " << fd.client_addr  << ":" << fd.client_port << std::endl;
                }
                auto cl = std::make_shared<BoostClientSession>(std::move(socket));
                cl->async_send(proto_project::dte::FirstData, fd.serialize());
                cl->start_session();
            }
            ListenTcp();
        }
    );
}