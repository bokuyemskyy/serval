#include "http_connection_handler.hpp"

#include "../include/http_connection.hpp"
#include "../include/http_request.hpp"
#include "http_response.hpp"
#include "socket.hpp"

HttpConnectionHandler::HttpConnectionHandler() = default;
void HttpConnectionHandler::setRequestHandler(IHttpRequestHandler* request_handler) {m_request_handler = request_handler;}

void HttpConnectionHandler::handleConnection(HttpConnection conn) {
    if (m_request_handler == nullptr) throw std::invalid_argument("HttpRequestHandler is nullptr");

    HttpRequest  req  = conn.readRequest();
    HttpResponse resp = m_request_handler->handleRequest(req);

    conn.writeResponse(resp);
    conn.close();
}