#include "../include/http_protocol.hpp"

#include "../include/http_connection_exception.hpp"

HttpProtocol::HttpProtocol(HttpServerConfig& config) : m_config(config) {}

HttpRequest HttpProtocol::parseRequest(const std::string& raw_request) {
    HttpRequest request;
    return request;
}
bool HttpProtocol::isRequestComplete(const std::string& raw_request) {
    // Check whether header was received completely
    size_t header_end = raw_request.find("\r\n\r\n");

    if (header_end == std::string::npos) {
        if (raw_request.size() > m_config.MAX_HEADER_BYTES) {
            throw RequestHeaderTooLargeException();
        }
        return false;
    }

    if (header_end > m_config.MAX_HEADER_BYTES) {
        throw RequestHeaderTooLargeException();
    }


    // Check for Content-Length header
    std::string raw_lower = raw_request;
    std::transform(raw_lower.begin(), raw_lower.end(), raw_lower.begin(), ::tolower);

    size_t cl_pos = raw_lower.find("content-length:");

    if (cl_pos == std::string::npos) {
        return true;  // No Content-Length, headers are enough
    }

    // Extract Content-Length value
    cl_pos += 15;

    while (cl_pos < raw_request.length() && (raw_request[cl_pos] == ' ' || raw_request[cl_pos] == '\t')) {
        cl_pos++;
    }

    // Find end of line
    size_t line_end = raw_request.find("\r\n", cl_pos);
    if (line_end == std::string::npos) {
        return false;  // Still not ready
    }

    // Parse the len as number
    size_t content_length = 0;
    try {
        std::string value = raw_request.substr(cl_pos, line_end - cl_pos);
        content_length = std::stoull(value);
    } catch (...) {
        return true; // Treat invalid length as no body
    }

    if (content_length > m_config.MAX_BODY_BYTES) {
        throw RequestBodyTooLargeException();
    }

    // Check if we have the full body
    size_t body_start = header_end + 4;  // After "\r\n\r\n"
    size_t body_received = raw_request.length() - body_start;

    return body_received >= content_length;
}