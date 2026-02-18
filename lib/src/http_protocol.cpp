#include "http_protocol.hpp"

#include "http_connection_exception.hpp"

#include <sstream>

HttpProtocol::HttpProtocol(HttpServerConfig& config) : m_config(config) {}

HttpRequest HttpProtocol::parseRequest(const std::string& raw_request) const {
    HttpRequest::Builder builder;

    // Split headers from body
    size_t header_end = raw_request.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        throw HttpConnectionException("Malformed request: no header terminator");
    }

    std::string header_section = raw_request.substr(0, header_end);
    std::string body           = raw_request.substr(header_end + 4);

    // Split header section into lines
    std::vector<std::string> lines;
    size_t                   pos = 0;
    while (pos < header_section.size()) {
        size_t end = header_section.find("\r\n", pos);
        if (end == std::string::npos)
            end = header_section.size();
        lines.push_back(header_section.substr(pos, end - pos));
        pos = end + 2;
    }

    if (lines.empty()) {
        throw HttpConnectionException("Malformed request: empty request");
    }

    const std::string& request_line = lines[0];

    size_t first_space  = request_line.find(' ');
    size_t second_space = request_line.find(' ', first_space + 1);

    if (first_space == std::string::npos || second_space == std::string::npos) {
        throw HttpConnectionException("Malformed request line: " + request_line);
    }

    // Method
    std::string method_str = request_line.substr(0, first_space);
    if (method_str == "GET")
        builder.setMethod(HttpMethod::GET);
    else if (method_str == "POST")
        builder.setMethod(HttpMethod::POST);
    else if (method_str == "PUT")
        builder.setMethod(HttpMethod::PUT);
    else if (method_str == "PATCH")
        builder.setMethod(HttpMethod::PATCH);
    else if (method_str == "DELETE")
        builder.setMethod(HttpMethod::DELETE);
    else if (method_str == "HEAD")
        builder.setMethod(HttpMethod::HEAD);
    else if (method_str == "OPTIONS")
        builder.setMethod(HttpMethod::OPTIONS);
    else if (method_str == "TRACE")
        builder.setMethod(HttpMethod::TRACE);
    else if (method_str == "CONNECT")
        builder.setMethod(HttpMethod::CONNECT);
    else
        builder.setMethod(HttpMethod::UNKNOWN);

    // Raw URL
    std::string raw_url = request_line.substr(first_space + 1, second_space - first_space - 1);
    builder.setRawUrl(raw_url);

    std::string path = raw_url;
    std::string query_string;
    std::string fragment;

    size_t fragment_pos = path.find('#');
    if (fragment_pos != std::string::npos) {
        fragment = path.substr(fragment_pos + 1);
        path     = path.substr(0, fragment_pos);
    }

    size_t query_pos = path.find('?');
    if (query_pos != std::string::npos) {
        query_string = path.substr(query_pos + 1);
        path         = path.substr(0, query_pos);
    }

    builder.setPath(path);
    builder.setQueryString(query_string);
    builder.setFragment(fragment);

    // Query parameters
    if (!query_string.empty()) {
        auto url_decode = [](const std::string& encoded) -> std::string {
            std::string decoded;
            for (size_t i = 0; i < encoded.size(); ++i) {
                if (encoded[i] == '+') {
                    decoded += ' ';
                } else if (encoded[i] == '%' && i + 2 < encoded.size()) {
                    int hex = std::stoi(encoded.substr(i + 1, 2), nullptr, 16);
                    decoded += static_cast<char>(hex);
                    i += 2;
                } else {
                    decoded += encoded[i];
                }
            }
            return decoded;
        };

        size_t qpos = 0;
        while (qpos < query_string.size()) {
            size_t amp = query_string.find('&', qpos);
            if (amp == std::string::npos)
                amp = query_string.size();

            std::string pair = query_string.substr(qpos, amp - qpos);
            size_t      eq   = pair.find('=');
            if (eq != std::string::npos) {
                builder.addQueryParameter(url_decode(pair.substr(0, eq)), url_decode(pair.substr(eq + 1)));
            } else if (!pair.empty()) {
                builder.addQueryParameter(url_decode(pair), "");
            }

            qpos = amp + 1;
        }
    }

    // Version
    std::string version_str = request_line.substr(second_space + 1);
    if (version_str == "HTTP/1.0")
        builder.setVersion(HttpVersion::HTTP_1_0);
    else if (version_str == "HTTP/1.1")
        builder.setVersion(HttpVersion::HTTP_1_1);
    else if (version_str == "HTTP/2.0")
        builder.setVersion(HttpVersion::HTTP_2_0);
    else if (version_str == "HTTP/3.0")
        builder.setVersion(HttpVersion::HTTP_3_0);
    else
        builder.setVersion(HttpVersion::UNKNOWN);

    // Headers
    for (size_t i = 1; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty())
            continue;

        size_t colon = line.find(':');
        if (colon == std::string::npos)
            continue;

        std::string key   = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        size_t val_start = value.find_first_not_of(" \t");
        if (val_start != std::string::npos)
            value = value.substr(val_start);

        builder.addHeader(key, value);

        // Cookies
        std::string key_lower = key;
        std::ranges::transform(key_lower, key_lower.begin(), ::tolower);
        if (key_lower == "cookie") {
            size_t cpos = 0;
            while (cpos < value.size()) {
                size_t semi = value.find(';', cpos);
                if (semi == std::string::npos)
                    semi = value.size();

                std::string cookie_pair = value.substr(cpos, semi - cpos);

                size_t cs = cookie_pair.find_first_not_of(" \t");
                if (cs != std::string::npos)
                    cookie_pair = cookie_pair.substr(cs);

                size_t ceq = cookie_pair.find('=');
                if (ceq != std::string::npos) {
                    builder.addCookie(cookie_pair.substr(0, ceq), cookie_pair.substr(ceq + 1));
                }

                cpos = semi + 1;
            }
        }
    }

    // Body (trim to Content-Length)
    std::string final_body     = body;
    auto        request_so_far = builder.build();
    if (request_so_far.hasHeader("Content-Length")) {
        try {
            size_t content_length = std::stoull(request_so_far.header("Content-Length"));
            if (content_length < final_body.size())
                final_body = final_body.substr(0, content_length);
        } catch (...) {
        }
    }
    builder.setBody(final_body);

    return builder.build();
}
std::string HttpProtocol::serializeResponse(const HttpResponse& response) const {
    std::ostringstream out;

    out << "HTTP/1.1 " << static_cast<int>(response.status) << " " << response.reasonPhrase << "\r\n";

    bool has_content_length = false;

    for (const auto& [key, value] : response.headers) {
        if (key == "Content-Length") {
            has_content_length = true;
        }
        out << key << ": " << value << "\r\n";
    }

    if (!has_content_length) {
        out << "Content-Length: " << response.body.size() << "\r\n";
    }

    out << "\r\n";

    out << response.body;

    return out.str();
}

bool HttpProtocol::isRequestComplete(const std::string& raw_request) const {
    // Check whether header was received completely
    size_t header_end = raw_request.find("\r\n\r\n");

    if (header_end == std::string::npos) {
        if (raw_request.size() > m_config.max_header_bytes) {
            throw RequestHeaderTooLargeException();
        }
        return false;
    }

    if (header_end > m_config.max_header_bytes) {
        throw RequestHeaderTooLargeException();
    }

    // Check for Content-Length header
    std::string raw_lower = raw_request;
    std::ranges::transform(raw_lower, raw_lower.begin(), ::tolower);

    size_t cl_pos = raw_lower.find("content-length:");

    if (cl_pos == std::string::npos) {
        return true; // No Content-Length, headers are enough
    }

    // Extract Content-Length value
    cl_pos += 15;

    while (cl_pos < raw_request.length() && (raw_request[cl_pos] == ' ' || raw_request[cl_pos] == '\t')) {
        cl_pos++;
    }

    // Find end of line
    size_t line_end = raw_request.find("\r\n", cl_pos);
    if (line_end == std::string::npos) {
        return false; // Still not ready
    }

    // Parse the len as number
    size_t content_length = 0;
    try {
        std::string value = raw_request.substr(cl_pos, line_end - cl_pos);
        content_length    = std::stoull(value);
    } catch (...) {
        return true; // Treat invalid length as no body
    }

    if (content_length > m_config.max_body_bytes) {
        throw RequestBodyTooLargeException();
    }

    // Check if we have the full body
    size_t body_start    = header_end + 4; // After "\r\n\r\n"
    size_t body_received = raw_request.length() - body_start;

    return body_received >= content_length;
}
bool HttpProtocol::shouldKeepAlive(const HttpRequest& request) const {
    std::string connection = request.header("Connection");
    std::ranges::transform(connection, connection.begin(), ::tolower);

    if (request.version == HttpVersion::HTTP_1_1) {
        // Keep-alive is the default for HTTP/1.1
        return connection != "close";
    }

    if (request.version == HttpVersion::HTTP_1_0) {
        // Keep-alive must be requested for HTTP/1.0
        return connection == "keep-alive";
    }

    return false;
}