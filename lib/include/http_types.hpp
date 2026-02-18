#pragma once

#include <algorithm>
#include <map>
#include <string>

/**
 * The method of HTTP request
 * The HTTP_ prefix prevents name collisions with Windows system macros
 */
enum class HttpMethod {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_PATCH,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_CONNECT,
    UNKNOWN
};

/**
 *  The HTTP protocol version to use.
 *  This project's HttpProtocol class does not support HTTP/2 and HTTP/3.
 *  Requests with a version higher than HTTP/1.1 will be implicitly downgraded to HTTP/1.1.
 */
enum class HttpVersion {
    HTTP_1_0, ///< Requests require explicit "Connection: keep-alive" header to make the connection keep-alive.
    HTTP_1_1, ///< Requests are keep-alive by default. Requires "Connection: close" request header to close it.
    HTTP_2_0, ///< Not supported
    HTTP_3_0, ///< Not supported
    UNKNOWN
};

/**
 * The HTTP response status.
 * Has a helper method to convert to reason phrase.
 *
 * @see toReasonPhrase()
 */
enum class HttpStatus {
    // Informational
    HTTP_CONTINUE            = 100,
    HTTP_SWITCHING_PROTOCOLS = 101,

    // Success
    HTTP_OK              = 200,
    HTTP_CREATED         = 201,
    HTTP_ACCEPTED        = 202,
    HTTP_NO_CONTENT      = 204,
    HTTP_PARTIAL_CONTENT = 206,

    // Redirection
    HTTP_MOVED_PERMANENTLY  = 301,
    HTTP_FOUND              = 302,
    HTTP_SEE_OTHER          = 303,
    HTTP_NOT_MODIFIED       = 304,
    HTTP_TEMPORARY_REDIRECT = 307,
    HTTP_PERMANENT_REDIRECT = 308,

    // Client errors
    HTTP_BAD_REQUEST            = 400,
    HTTP_UNAUTHORIZED           = 401,
    HTTP_FORBIDDEN              = 403,
    HTTP_NOT_FOUND              = 404,
    HTTP_METHOD_NOT_ALLOWED     = 405,
    HTTP_REQUEST_TIMEOUT        = 408,
    HTTP_CONFLICT               = 409,
    HTTP_GONE                   = 410,
    HTTP_PAYLOAD_TOO_LARGE      = 413,
    HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_TOO_MANY_REQUESTS      = 429,

    // Server errors
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED       = 501,
    HTTP_BAD_GATEWAY           = 502,
    HTTP_SERVICE_UNAVAILABLE   = 503,
    HTTP_GATEWAY_TIMEOUT       = 504
};

/**
 * Converts an HttpStatus enum value to its standard reason phrase string.
 */
inline const char* toReasonPhrase(HttpStatus status) {
    switch (status) {
        case HttpStatus::HTTP_CONTINUE:
            return "Continue";
        case HttpStatus::HTTP_SWITCHING_PROTOCOLS:
            return "Switching Protocols";
        case HttpStatus::HTTP_OK:
            return "OK";
        case HttpStatus::HTTP_CREATED:
            return "Created";
        case HttpStatus::HTTP_ACCEPTED:
            return "Accepted";
        case HttpStatus::HTTP_NO_CONTENT:
            return "No Content";
        case HttpStatus::HTTP_PARTIAL_CONTENT:
            return "Partial Content";
        case HttpStatus::HTTP_MOVED_PERMANENTLY:
            return "Moved Permanently";
        case HttpStatus::HTTP_FOUND:
            return "Found";
        case HttpStatus::HTTP_SEE_OTHER:
            return "See Other";
        case HttpStatus::HTTP_NOT_MODIFIED:
            return "Not Modified";
        case HttpStatus::HTTP_TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case HttpStatus::HTTP_PERMANENT_REDIRECT:
            return "Permanent Redirect";
        case HttpStatus::HTTP_BAD_REQUEST:
            return "Bad Request";
        case HttpStatus::HTTP_UNAUTHORIZED:
            return "Unauthorized";
        case HttpStatus::HTTP_FORBIDDEN:
            return "Forbidden";
        case HttpStatus::HTTP_NOT_FOUND:
            return "Not Found";
        case HttpStatus::HTTP_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HttpStatus::HTTP_REQUEST_TIMEOUT:
            return "Request Timeout";
        case HttpStatus::HTTP_CONFLICT:
            return "Conflict";
        case HttpStatus::HTTP_GONE:
            return "Gone";
        case HttpStatus::HTTP_PAYLOAD_TOO_LARGE:
            return "Payload Too Large";
        case HttpStatus::HTTP_UNSUPPORTED_MEDIA_TYPE:
            return "Unsupported Media Type";
        case HttpStatus::HTTP_TOO_MANY_REQUESTS:
            return "Too Many Requests";
        case HttpStatus::HTTP_INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case HttpStatus::HTTP_NOT_IMPLEMENTED:
            return "Not Implemented";
        case HttpStatus::HTTP_BAD_GATEWAY:
            return "Bad Gateway";
        case HttpStatus::HTTP_SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case HttpStatus::HTTP_GATEWAY_TIMEOUT:
            return "Gateway Timeout";

        default:
            return "Unknown";
    }
}

/**
 * Used in header map type
 * Headers are case-insensitive
 */
struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::ranges::lexicographical_compare(
            a, b, [](unsigned char ca, unsigned char cb) { return std::tolower(ca) < std::tolower(cb); });
    }
};

using HeaderMap = std::map<std::string, std::string, CaseInsensitiveCompare>;
using QueryMap  = std::map<std::string, std::string>;
using CookieMap = std::map<std::string, std::string>;