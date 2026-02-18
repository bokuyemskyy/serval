#include "http_request.hpp"

#include <string>

HttpRequest::HttpRequest() = default;

std::string HttpRequest::cookie(const std::string& name) const {
    auto it = cookies.find(name);
    return it != cookies.end() ? it->second : "";
}

bool HttpRequest::hasCookie(const std::string& name) const {
    return cookies.contains(name);
}

std::string HttpRequest::queryParameter(const std::string& key) const {
    auto it = query_parameters.find(key);
    return it != query_parameters.end() ? it->second : "";
}

bool HttpRequest::hasQueryParameter(const std::string& key) const {
    return query_parameters.contains(key);
}

std::string HttpRequest::header(const std::string& name) const {
    auto it = headers.find(name);
    return it != headers.end() ? it->second : "";
}

bool HttpRequest::hasHeader(const std::string& name) const {
    return headers.contains(name);
}