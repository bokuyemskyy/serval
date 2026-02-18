#include "http_connection_handler.hpp"

#include "http_connection.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

HttpConnectionHandler::HttpConnectionHandler(std::shared_ptr<IHttpRequestHandler> request_handler,
                                             HttpServerConfig&                    config)
    : m_request_handler(request_handler), m_config(config), m_protocol(m_config) {}

void HttpConnectionHandler::handle(std::shared_ptr<HttpConnection> conn) {
    if (!m_request_handler)
        throw std::invalid_argument("No HttpRequestHandler set");

    if (conn->state == HttpConnectionState::READING) {
        if (conn->recv()) {
            if (m_protocol.isRequestComplete(conn->buffer)) {
                HttpRequest request = m_protocol.parseRequest(conn->buffer);
                conn->keep_alive =
                    m_protocol.shouldKeepAlive(request); // Protocol decides on whether request makes the connection
                                                         // keep-alive based on the HTTP version per-request
                HttpResponse response = m_request_handler->handle(request);
                conn->startSend(m_protocol.serializeResponse(response));
            }
        }
    } else if (conn->state == HttpConnectionState::WRITING) {
        if (conn->send()) {
            if (conn->keep_alive)
                conn->startRecv();
            else
                conn->close();
        }
    }
}