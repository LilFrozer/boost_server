#include "ClientSession.h"
#include <iostream>

BoostClientSession::BoostClientSession(boost_tcp::socket socket) : socket_(std::move(socket))
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
            std::cerr << "Client disconnected" << std::endl;
            return;
        }
        buffer_.resize(size_);
        boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, size_t) {
            if (ec) {
                std::cerr << "Body read error: " << ec.message() << std::endl;
                return;
            }
            this->process_packet();
            this->async_read();
        });
    });
}

void BoostClientSession::process_packet()
{
    unsigned offset = 0;

    proto_project::Packet pkt{};

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
    memcpy(&pkt.d_type, &(this->buffer_)[offset], 2);
    offset += 2;

    pkt.buffer.assign((this->buffer_).begin() + offset, (this->buffer_).end());

    if ( pkt.header.server_hash != proto_project::kServerHash ) {
        std::cerr << "ERROR -> != kServerHash" << std::endl;
        return;
    }

    if ( pkt.header.isFirst != 1 || pkt.header.isLast != 1 ) {
        std::cerr << "ERROR -> !flags" << std::endl;
        return;
    }

    switch (pkt.d_type) {
        case tcp_data::DataTypes::TestStruct:
        {
            std::cout << "---TestStruct---" << std::endl;
            tcp_data::TestStruct a = tcp_data::TestStruct::deserialize(pkt.buffer.data());
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
            break;
        }
    }
}

void BoostClientSession::async_send(proto_project::dte dtype, const vU8 &buffer) {
    if( !socket_.is_open() )
    {
        std::cerr << "Connecting error!" << std::endl;
        return;
    }

    proto_project::Packet pkt{};
    pkt.header.server_hash = proto_project::kServerHash;
    pkt.header.total_cnt_packets = 1;
    pkt.header.total_data_size = buffer.size();
    pkt.header.cur_packet_size = buffer.size() + sizeof(proto_project::PacketHeader) + sizeof(u16);
    pkt.header.isFirst = 1;
    pkt.header.isLast = 1;
    pkt.d_type = dtype;
    pkt.buffer = buffer;

    vU8 full_packet = proto_project::Packet::serialize(pkt);

    u32 size = full_packet.size();
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(&size, 4));
    buffers.push_back(boost::asio::buffer(full_packet));

    boost::asio::async_write(this->socket_, buffers, [this](boost::system::error_code ec, size_t length) {
        if ( ec ) {
            std::cerr << "fck send!" << std::endl;
        } else {
            std::cout << "Sended " << length << "bytes" << std::endl;
        }
    });
}
