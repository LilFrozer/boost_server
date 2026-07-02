#include "ImitatorFar.h"
#include <stdexcept>

ImitatorFar::~ImitatorFar() {
    is_running_.store(false);
    if (listen_thread_.joinable()) {
        listen_thread_.join();
    }
}

SocketError ImitatorFar::Start() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        return SocketError::CREATE_ERROR;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(IMIT_PORT);
    inet_pton(AF_INET, IMIT_ADDR, &(addr.sin_addr));

    if (bind(socket_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        return SocketError::BIND_ERROR;
    }

    if (listen(socket_, 5) < 0) {
        return SocketError::LISTEN_ERROR;
    }

    listen_thread_ = std::thread(&ImitatorFar::Listen, this);
    listen_thread_.detach();

    return SocketError::SUCCESS;
}

void ImitatorFar::Listen() {
    while (is_running_.load()) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(socket_, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientLen);
        if (clientSocket < 0) {
            std::cerr << "[ImitatorFar]Client can`t connect!" << std::endl;
            continue;
        }

        char buffer[1024]{0};
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            std::cout << "[ImitatorFar] bytesRcv: " << buffer << std::endl;
        }
        close(clientSocket);
    }
    close(socket_);
}