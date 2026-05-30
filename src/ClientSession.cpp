//
// Created by Алексей Подоплелов on 25.05.2026.
//

#include "../include/ClientSession.h"
#include <iostream>

BoostClientSession::BoostClientSession(tcpSock socket) : socket_(std::move(socket))
{
}

void BoostClientSession::start_session()
{
    this->async_read();
}

void BoostClientSession::async_read() {
    auto self = shared_from_this();
    boost::asio::async_read(socket_, boost::asio::buffer(&size_, sizeof(size_)),
        [this, self](boost::system::error_code ec, size_t) {
            if (ec == boost::asio::error::eof) {
                std::cout << "Client disconnected" << std::endl;
                return;
            }
            buffer_.resize(size_);
            boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                [this, self](boost::system::error_code ec, size_t) {
                    if (ec) {
                        std::cerr << "Body read error: " << ec.message() << std::endl;
                        return;
                    }
                    process_packet();
                    async_read();
                });
        });
}

void BoostClientSession::process_packet()
{
    unsigned offset = 0;

    CONNECTION::Packet pkt;

    // !!!PARSING HEADER!!! ...ClientSession.h
    memcpy(&pkt.header.server_hash, &(this->buffer_)[offset], 2);
    offset += 2;
    memcpy(&pkt.header.total_data_size, &(this->buffer_)[offset], 4);
    offset += 4;
    memcpy(&pkt.header.total_cnt_packets, &(this->buffer_)[offset], 2);
    offset += 2;
    memcpy(&pkt.header.cur_packet_number, &(this->buffer_)[offset], 2);
    offset += 2;
    memcpy(&pkt.header.cur_packet_size, &(this->buffer_)[offset], 2);
    offset += 2;
    uint8_t flags = (this->buffer_)[offset];
    pkt.header.isFirst = (flags & 0x01) ? 1 : 0;
    pkt.header.isLast = (flags & 0x02) ? 1 : 0;
    offset += 2;
    memcpy(&pkt.data_type, &(this->buffer_)[offset], 2);
    offset += 2;

    /* !!!TRANSE DATA!!! */
    pkt.data.assign((this->buffer_).begin() + offset, (this->buffer_).end());

    if ( pkt.header.server_hash != CONNECTION::SERVER_HASH ) {
        std::cerr << "ERROR -> != SERVER_HASH" << std::endl;
        return;
    }

    if ( pkt.header.isFirst != 1 || pkt.header.isLast != 1 ) {
        std::cerr << "ERROR -> !flags" << std::endl;
        return;
    }

    switch (pkt.data_type) {
        case TCP_DATA::DataTypes::TestStruct:
        {
            TCP_DATA::TestStruct a = TCP_DATA::TestStruct::deserialize(pkt.data.data());
            std::cout << "---TestStruct---" << std::endl;
            std::cout << a.a << std::endl;
            std::cout << a.b << std::endl;
            std::cout << a.c << std::endl;
            for (size_t i=0;i<a.d.size();++i) {
                std::cout << a.d[i] << " ";
            }
            std::cout << std::endl;
            break;
        }
        default: {
            std::cerr << "wtf is this data" << std::endl;
        }
    }
}