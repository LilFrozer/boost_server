//
// Created by Алексей Подоплелов on 25.05.2026.
//

#ifndef MIL_CLIENTSESSION_H
#define MIL_CLIENTSESSION_H

#include <GroupData.h>

namespace CONNECTION
{
    const uint16_t SERVER_HASH = 0x1337;
    const uint16_t SERVER_PORT = 1122;

    struct PacketHeader
    {
        uint16_t server_hash{SERVER_HASH};

        uint32_t total_data_size{0x0};
        uint16_t total_cnt_packets{0x0};
        uint16_t cur_packet_number{0x0};
        uint16_t cur_packet_size{0x0};

        uint8_t isFirst : 1;
        uint8_t isLast : 1;
        uint8_t : 6;
    };

    struct Packet
    {
        PacketHeader header{};
        TCP_DATA::DataTypes data_type{TCP_DATA::DataTypes::TestStruct};
        std::vector<uint8_t> data{};
    };
}

class IClientSession
{
public:
    virtual ~IClientSession() {}
    virtual void start_session() = 0;
    virtual void async_read() = 0;
    virtual void process_packet() = 0;
};

#include <boost/asio.hpp>

using tcpSock = boost::asio::ip::tcp::socket;
using vecUint8 = std::vector<uint8_t>;

/**
 * bruhhh
 */
class BoostClientSession : public IClientSession, std::enable_shared_from_this<BoostClientSession>
{
private:
    tcpSock socket_{nullptr};
    unsigned size_{0};
    vecUint8 buffer_{};
public:
    BoostClientSession(tcpSock socket);
    ~BoostClientSession() override {};
    void start_session() override final;
    void async_read() override final;
    void process_packet() override final;
};


#endif //MIL_CLIENTSESSION_H