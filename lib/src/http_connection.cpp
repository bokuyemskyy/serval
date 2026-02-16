#include "../include/http_connection.hpp"

#include "../include/http_connection_exception.hpp"
#include "../include/http_request.hpp"
#include "../include/http_response.hpp"
#include "socket.hpp"

#include <thread>

HttpConnection::HttpConnection(Socket client, HttpProtocol& protocol, HttpServerConfig& config) : m_client(std::move(client)), m_protocol(protocol), m_config(config) {}

HttpRequest HttpConnection::readRequest()
{
    auto start = std::chrono::steady_clock::now();
    const auto TIMEOUT = std::chrono::milliseconds(m_config.REQUEST_TIMEOUT_MS);

    std::string raw_request;
    while (!m_protocol.isRequestComplete(raw_request)) {
        switch (auto [status, bytes] = m_client.recv(raw_request, 4096); status) {
            case IoStatus::DATA:
                continue;

            case IoStatus::WOULD_BLOCK:
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                break;

            case IoStatus::CLOSED:
                throw IncompleteRequestException();
        }

        if (std::chrono::steady_clock::now() - start > TIMEOUT) {
            throw RequestTimeoutException();
        }
    }

    return m_protocol.parseRequest(raw_request);
}

void HttpConnection::writeResponse(HttpResponse response) {
    auto start = std::chrono::steady_clock::now();
    const auto TIMEOUT = std::chrono::milliseconds(m_config.REQUEST_TIMEOUT_MS);

    const std::string raw_response = response.toString();
    size_t total_bytes_sent = 0;
    while (total_bytes_sent < raw_response.length()) {
        switch (auto [status, bytes] = m_client.send(raw_response); status) {
            case IoStatus::DATA:
                total_bytes_sent += bytes;
                continue;

            case IoStatus::WOULD_BLOCK:
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                break;

            case IoStatus::CLOSED:
                return;
        }

        if (std::chrono::steady_clock::now() - start > TIMEOUT) {
            return;
        }
    }
}

void HttpConnection::close() { m_client.close(); }