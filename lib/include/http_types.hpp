#pragma once

#include <algorithm>
#include <map>
#include <string>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT,
    UNKNOWN
};

enum class HttpVersion {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0, // Not supported
    HTTP_3_0, // Not supported
    UNKNOWN
};

enum class HttpStatus {

    // Informational
    CONTINUE            = 100,
    SWITCHING_PROTOCOLS = 101,

    // Success
    OK              = 200,
    CREATED         = 201,
    ACCEPTED        = 202,
    NO_CONTENT      = 204,
    PARTIAL_CONTENT = 206,

    // Redirection
    MOVED_PERMANENTLY  = 301,
    FOUND              = 302,
    SEE_OTHER          = 303,
    NOT_MODIFIED       = 304,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,

    // Client errors
    BAD_REQUEST            = 400,
    UNAUTHORIZED           = 401,
    FORBIDDEN              = 403,
    NOT_FOUND              = 404,
    METHOD_NOT_ALLOWED     = 405,
    REQUEST_TIMEOUT        = 408,
    CONFLICT               = 409,
    GONE                   = 410,
    PAYLOAD_TOO_LARGE      = 413,
    UNSUPPORTED_MEDIA_TYPE = 415,
    TOO_MANY_REQUESTS      = 429,

    // Server errors
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED       = 501,
    BAD_GATEWAY           = 502,
    SERVICE_UNAVAILABLE   = 503,
    GATEWAY_TIMEOUT       = 504
};

inline const char* toReasonPhrase(HttpStatus status) {
    switch (status) {
        case HttpStatus::CONTINUE:
            return "Continue";
        case HttpStatus::SWITCHING_PROTOCOLS:
            return "Switching Protocols";
        case HttpStatus::OK:
            return "OK";
        case HttpStatus::CREATED:
            return "Created";
        case HttpStatus::ACCEPTED:
            return "Accepted";
        case HttpStatus::NO_CONTENT:
            return "No Content";
        case HttpStatus::PARTIAL_CONTENT:
            return "Partial Content";
        case HttpStatus::MOVED_PERMANENTLY:
            return "Moved Permanently";
        case HttpStatus::FOUND:
            return "Found";
        case HttpStatus::SEE_OTHER:
            return "See Other";
        case HttpStatus::NOT_MODIFIED:
            return "Not Modified";
        case HttpStatus::TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case HttpStatus::PERMANENT_REDIRECT:
            return "Permanent Redirect";
        case HttpStatus::BAD_REQUEST:
            return "Bad Request";
        case HttpStatus::UNAUTHORIZED:
            return "Unauthorized";
        case HttpStatus::FORBIDDEN:
            return "Forbidden";
        case HttpStatus::NOT_FOUND:
            return "Not Found";
        case HttpStatus::METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HttpStatus::REQUEST_TIMEOUT:
            return "Request Timeout";
        case HttpStatus::CONFLICT:
            return "Conflict";
        case HttpStatus::GONE:
            return "Gone";
        case HttpStatus::PAYLOAD_TOO_LARGE:
            return "Payload Too Large";
        case HttpStatus::UNSUPPORTED_MEDIA_TYPE:
            return "Unsupported Media Type";
        case HttpStatus::TOO_MANY_REQUESTS:
            return "Too Many Requests";
        case HttpStatus::INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case HttpStatus::NOT_IMPLEMENTED:
            return "Not Implemented";
        case HttpStatus::BAD_GATEWAY:
            return "Bad Gateway";
        case HttpStatus::SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case HttpStatus::GATEWAY_TIMEOUT:
            return "Gateway Timeout";

        default:
            return "Unknown";
    }
}

struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::ranges::lexicographical_compare(
            a, b, [](unsigned char ca, unsigned char cb) { return std::tolower(ca) < std::tolower(cb); });
    }
};

using HeaderMap = std::map<std::string, std::string, CaseInsensitiveCompare>;
using QueryMap  = std::map<std::string, std::string>;
using CookieMap = std::map<std::string, std::string>;