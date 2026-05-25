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
    shared_from_this()->async_read();
}

void BoostClientSession::async_read()
{
    size_t sizeH = 4;   // headerSize == 4bytes

    /*
     *  !!!READING SIZE && READING FULL PACKET USING SIZE!!!
     */
    boost::asio::async_read(this->socket_,
                    boost::asio::buffer(&(this->size_), sizeH),
                    [this](boost::system::error_code ec, size_t)
    {
        if ( !ec ) {
            this->buffer_.resize(this->size_);
            boost::asio::async_read(this->socket_,
            boost::asio::buffer(this->buffer_),
            [this](boost::system::error_code ec, size_t)
            {
                if ( !ec ) {
                    this->process_packet();
                    this->async_read();
                } else {
                    std::cerr << "ec -> " << ec.message() << std::endl;
                }
            });
        } else {
            std::cerr << "ec -> " << ec.message() << std::endl;
        }
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
    }
}