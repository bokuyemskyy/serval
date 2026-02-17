#pragma once

#include <string>
#include <algorithm>
#include <map>

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
    HTTP_2_0,
    UNKNOWN
};

struct CaseInsensitiveCompare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::ranges::lexicographical_compare(
            a, b,
            [](unsigned char ca, unsigned char cb) {
                return std::tolower(ca) < std::tolower(cb);
            }
        );
    }
};

using HeaderMap = std::map<std::string, std::string, CaseInsensitiveCompare>;
using QueryMap  = std::map<std::string, std::string>;
using CookieMap = std::map<std::string, std::string>;