
#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <cstdint>

#define IMIT_PORT 10001
#define IMIT_ADDR "192.168.1.73"

enum class SocketError : int16_t {
    SUCCESS = 1,
    CREATE_ERROR = 0,
    BIND_ERROR = -1,
    LISTEN_ERROR = -2
};

struct farbos_hdr {
    unsigned dk : 20;
    unsigned tk : 4;
    unsigned prmbl : 8;

    unsigned snp;

    unsigned : 16;
    unsigned marker : 8;
    unsigned : 8;

    unsigned crc : 16;
    unsigned py : 16;
};

struct viod_hdr {
    uint16_t slid : 2,
            spid : 2,
            smid : 4,
            dlid : 2,
            dpid : 2,
            dmid : 4;
    uint16_t prmbl;
    uint16_t reserved1 : 2,
            offset : 14;
    uint16_t length;
    uint16_t full_length;
    uint16_t chid : 4,
            e : 1,
            f : 1,
            reserved2 : 10;
    uint16_t ip_port;
    uint16_t ip_addr;
};
static_assert(sizeof(viod_hdr)==16, "sizeof(viod_hdr)!=16");
static_assert(sizeof(farbos_hdr)==16, "sizeof(farbos_hdr)!=16");

class IImitator {
public:
    virtual SocketError Start() = 0;
    virtual void Listen() = 0;
    virtual ~IImitator() {};
};

class ImitatorFar : public IImitator {
protected:
    std::atomic<bool> is_running_{true};
    int socket_{-1};
    std::thread listen_thread_;
public:
    SocketError Start() override;
    void Listen() override;
    ~ImitatorFar() override;
};
