#pragma once
#include "http_types.hpp"

/**
 * Represents an HTTP request with method, http version, raw url and its parts for convenience, headers, query
 * parameters, cookies and body.
 *
 * Is an immutable data structure with a builder.
 * Does not provide any network functionality, thus should be used in conjunction with HttpProtocol class
 */
class HttpRequest {
  public:
    HttpMethod         method() const;
    HttpVersion        version() const;
    const std::string& rawUrl() const;
    const std::string& path() const;
    const std::string& queryString() const;
    const std::string& fragment() const;
    const HeaderMap&   headers() const;
    const QueryMap&    queryParameters() const;
    const CookieMap&   cookies() const;
    const std::string& body() const;

    std::string header(const std::string& name) const;
    bool        hasHeader(const std::string& name) const;

    std::string cookie(const std::string& name) const;
    bool        hasCookie(const std::string& name) const;

    std::string queryParameter(const std::string& key) const;
    bool        hasQueryParameter(const std::string& key) const;

    class Builder;

  private:
    HttpRequest();

    HttpMethod  m_method  = HttpMethod::UNKNOWN;
    HttpVersion m_version = HttpVersion::HTTP_1_1;

    std::string m_raw_url;      // Full raw URL as received, e.g. "/search?q=hello#anchor"
    std::string m_path;         // The path part
    std::string m_query_string; // The query part
    std::string m_fragment;     // The fragment #part (rarely used)

    HeaderMap m_headers;
    QueryMap  m_query_parameters;
    CookieMap m_cookies;

    std::string m_body;
};

class HttpRequest::Builder {
  public:
    Builder& setMethod(HttpMethod m) {
        req.m_method = m;
        return *this;
    }

    Builder& setVersion(HttpVersion v) {
        req.m_version = v;
        return *this;
    }

    Builder& setRawUrl(const std::string& url) {
        req.m_raw_url = url;
        return *this;
    }

    Builder& setPath(const std::string& p) {
        req.m_path = p;
        return *this;
    }

    Builder& setQueryString(const std::string& qs) {
        req.m_query_string = qs;
        return *this;
    }

    Builder& setFragment(const std::string& f) {
        req.m_fragment = f;
        return *this;
    }

    Builder& addHeader(const std::string& key, const std::string& value) {
        req.m_headers[key] = value;
        return *this;
    }

    Builder& addQueryParameter(const std::string& key, const std::string& value) {
        req.m_query_parameters[key] = value;
        return *this;
    }

    Builder& addCookie(const std::string& key, const std::string& value) {
        req.m_cookies[key] = value;
        return *this;
    }

    Builder& setBody(const std::string& b) {
        req.m_body = b;
        return *this;
    }

    HttpRequest build() { return req; }

  private:
    HttpRequest req;
};
