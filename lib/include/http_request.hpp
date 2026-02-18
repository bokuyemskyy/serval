#pragma once
#include "http_types.hpp"

struct HttpRequest {
    HttpRequest();

    HttpMethod  method  = HttpMethod::UNKNOWN;
    HttpVersion version = HttpVersion::HTTP_1_1;

    std::string rawUrl;       // Full raw URL as received, e.g. "/search?q=hello#anchor"
    std::string path;         // The path part
    std::string query_string; // The query part
    std::string fragment;     // The fragment #part (rarely used)

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

class HttpRequest::Builder {
  public:
    Builder& setMethod(HttpMethod m) {
        req.method = m;
        return *this;
    }

    Builder& setVersion(HttpVersion v) {
        req.version = v;
        return *this;
    }

    Builder& setRawUrl(const std::string& url) {
        req.rawUrl = url;
        return *this;
    }

    Builder& setPath(const std::string& p) {
        req.path = p;
        return *this;
    }

    Builder& setQueryString(const std::string& qs) {
        req.query_string = qs;
        return *this;
    }

    Builder& setFragment(const std::string& f) {
        req.fragment = f;
        return *this;
    }

    Builder& addHeader(const std::string& key, const std::string& value) {
        req.headers[key] = value;
        return *this;
    }

    Builder& addQueryParameter(const std::string& key, const std::string& value) {
        req.query_parameters[key] = value;
        return *this;
    }

    Builder& addCookie(const std::string& key, const std::string& value) {
        req.cookies[key] = value;
        return *this;
    }

    Builder& setBody(const std::string& b) {
        req.body = b;
        return *this;
    }

    HttpRequest build() { return req; }

  private:
    HttpRequest req;
};
