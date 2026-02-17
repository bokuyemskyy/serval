#pragma once

#include "http_request.hpp"
#include "http_protocol.hpp"
#include "http_response.hpp"
#include "socket.hpp"

enum class HttpConnectionState {
    WRITING,
    READING,
    CLOSING
};

struct HttpConnection {
    HttpConnection(Socket client);

    int fd() const;


    HttpRequest readRequest();
    void        writeResponse(HttpResponse response);

    void close();

    Socket client;

    HttpConnectionState state{HttpConnectionState::READING};
};