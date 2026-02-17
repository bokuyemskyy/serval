#include "../include/http_connection.hpp"

#include "../include/http_connection_exception.hpp"
#include "../include/http_request.hpp"
#include "../include/http_response.hpp"
#include "socket.hpp"

#include <thread>

HttpConnection::HttpConnection(Socket client) : client(std::move(client)) {}
int HttpConnection::fd() const {
    return client.fd();
}

void HttpConnection::startSend(std::string raw) {
    send_offset = 0;
    buffer      = std::move(raw);
    state       = HttpConnectionState::WRITING;
}
void HttpConnection::startRecv() {
    buffer.clear();
    state = HttpConnectionState::READING;
}

bool HttpConnection::recv() {
    auto [status, bytes] = client.recv(buffer, 4096);

    switch (status) {
        case IoStatus::DATA:
            last_recv = std::chrono::steady_clock::now();
            return true;
        case IoStatus::WOULD_BLOCK:
            // This case is unlikely because we received the poll signal previously
            close();
            throw WouldBlockException();
        case IoStatus::CLOSED:
            close();
            throw IncompleteRequestException();
    }

    return false;
}

bool HttpConnection::send() {
    auto [status, bytes] = client.send(buffer.data() + send_offset, buffer.size() - send_offset);

    switch (status) {
        case IoStatus::DATA:
            send_offset += bytes;
            return send_offset >= buffer.size();
        case IoStatus::WOULD_BLOCK:
            // This case is unlikely because we received the poll signal previously
            close();
            throw WouldBlockException();
        case IoStatus::CLOSED:
            close();
            throw IncompleteRequestException();
    }

    return false;
}

void HttpConnection::close() {
    state = HttpConnectionState::CLOSING;
}