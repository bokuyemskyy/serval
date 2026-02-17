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

class HttpRequest::Builder {
    HttpRequest req;

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
        req.queryString = qs;
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
};