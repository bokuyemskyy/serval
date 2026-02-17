#pragma once

#include "http_protocol.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "socket.hpp"

enum class HttpConnectionState {
    WRITING,
    READING,
    CLOSING
};

struct HttpConnection {
    using TimePoint = std::chrono::steady_clock::time_point;

    HttpConnection(Socket client);

    int fd() const;

    void startRecv();
    void startSend(std::string raw);
    void close();

    bool recv();
    bool send();

    Socket client;

    std::string buffer;
    size_t      send_offset;
    TimePoint   last_recv{std::chrono::steady_clock::now()};

    bool                keep_alive{false};
    HttpConnectionState state{HttpConnectionState::READING};
};