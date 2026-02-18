#include "http_request.hpp"

#include <string>

HttpRequest::HttpRequest() = default;

HttpMethod HttpRequest::method() const {
    return m_method;
}
HttpVersion HttpRequest::version() const {
    return m_version;
}
const std::string& HttpRequest::rawUrl() const {
    return m_raw_url;
}
const std::string& HttpRequest::path() const {
    return m_path;
}
const std::string& HttpRequest::queryString() const {
    return m_query_string;
}
const std::string& HttpRequest::fragment() const {
    return m_fragment;
}
const HeaderMap& HttpRequest::headers() const {
    return m_headers;
}
const QueryMap& HttpRequest::queryParameters() const {
    return m_query_parameters;
}
const CookieMap& HttpRequest::cookies() const {
    return m_cookies;
}
const std::string& HttpRequest::body() const {
    return m_body;
}

std::string HttpRequest::header(const std::string& name) const {
    auto it = m_headers.find(name);
    return it != m_headers.end() ? it->second : "";
}

bool HttpRequest::hasHeader(const std::string& name) const {
    return m_headers.contains(name);
}

std::string HttpRequest::cookie(const std::string& name) const {
    auto it = m_cookies.find(name);
    return it != m_cookies.end() ? it->second : "";
}

bool HttpRequest::hasCookie(const std::string& name) const {
    return m_cookies.contains(name);
}

std::string HttpRequest::queryParameter(const std::string& key) const {
    auto it = m_query_parameters.find(key);
    return it != m_query_parameters.end() ? it->second : "";
}

bool HttpRequest::hasQueryParameter(const std::string& key) const {
    return m_query_parameters.contains(key);
}
