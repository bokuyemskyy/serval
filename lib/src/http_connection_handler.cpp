#include "http_connection_handler.hpp"

#include "../include/http_connection.hpp"
#include "../include/http_request.hpp"
#include "http_response.hpp"

HttpConnectionHandler::HttpConnectionHandler(std::shared_ptr<IHttpRequestHandler> request_handler,
                                             HttpServerConfig&                    config)
    : m_request_handler(request_handler), m_config(config), m_protocol(m_config) {}

void HttpConnectionHandler::handle(std::shared_ptr<HttpConnection> conn) const {
    if (!m_request_handler)
        throw std::invalid_argument("No HttpRequestHandler set");

    HttpRequest  req  = conn->readRequest();
    HttpResponse resp = m_request_handler->handleRequest(req);
    conn->writeResponse(resp);
    conn->close();
}