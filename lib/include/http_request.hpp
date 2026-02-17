#pragma once
#include "http_types.hpp"

struct HttpRequest {
    HttpRequest();

    HttpMethod  method  = HttpMethod::UNKNOWN;
    HttpVersion version = HttpVersion::HTTP_1_1;

    std::string rawUrl;      // Full raw URL as received, e.g. "/search?q=hello#anchor"
    std::string path;        // The path part
    std::string queryString; // The query part
    std::string fragment;    // The fragment #part (rarely used)

    HeaderMap headers;
    QueryMap  query_parameters;
    CookieMap cookies;

    std::string body;

    std::string header(const std::string& name) const;

    bool hasHeader(const std::string& name) const;

    std::string cookie(const std::string& name) const;
    bool        hasCookie(const std::string& name) const;

    std::string queryParameter(const std::string& key) const;
    bool        hasQueryParameter(const std::string& key) const;

    class Builder;
};