#pragma once

#include "http_protocol.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "socket.hpp"

/**
 * Represents the state of an HTTP connection.
 */
enum class HttpConnectionState {
    WRITING, ///< Connection has some content in its buffer and did not write it fully yet.
    READING, ///< Connection did not receive the full request to its buffer yet.
    CLOSING  ///< Connection is about to close.
};

/**
 * @brief Represents a single HTTP client connection.
 *
 * Manages the socket, I/O buffer, connection state, and keep-alive status.
 * Used internally by HttpServer to track active connections.
 */
struct HttpConnection {
    using TimePoint = std::chrono::steady_clock::time_point;

    /**
     * @brief Construct a new HttpConnection for a client socket.
     *
     * @param client The client socket associated with this connection.
     */
    HttpConnection(Socket client);

    /// Returns the underlying socket file descriptor.
    int fd() const;

    /// Begin receiving data from the client.
    void startRecv();

    /// Begin sending data to the client.
    /// @param raw The raw HTTP response data to send.
    void startSend(std::string raw);

    /// Close the connection and clean up resources.
    void close();

    /// Perform a read from the client socket.
    /// @return true if ANY data was read successfully.
    bool recv();

    /// Perform a write to the client socket.
    /// @return true if ALL data was sent successfully.
    bool send();

    Socket client;

    std::string buffer;
    size_t      send_offset;
    TimePoint   last_recv{std::chrono::steady_clock::now()};

    bool                keep_alive{false};
    HttpConnectionState state{HttpConnectionState::READING};
};