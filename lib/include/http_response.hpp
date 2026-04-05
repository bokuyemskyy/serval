#pragma once

#include "http_types.hpp"

#include <map>
#include <string>

/**
 * Represents an HTTP response with status code, headers, and body.
 *
 * Is an immutable data structure with a builder.
 * Does not provide any network functionality, thus should be used in conjunction with HttpProtocol class
 */
class HttpResponse {
  public:
    HttpVersion        version() const;
    HttpStatus         status() const;
    const std::string& reasonPhrase() const;
    const HeaderMap&   headers() const;
    const std::string& body() const;

    std::string header(const std::string& name) const;
    bool        hasHeader(const std::string& name) const;

    class Builder;

    // Common responses
    static HttpResponse ok(std::string body = "", std::string ct = "text/plain");
    static HttpResponse json(std::string body);
    static HttpResponse html(std::string body);
    static HttpResponse created(std::string body = "");
    static HttpResponse noContent();
    static HttpResponse badRequest(std::string body = "Bad Request");
    static HttpResponse unauthorized(std::string body = "Unauthorized");
    static HttpResponse forbidden(std::string body = "Forbidden");
    static HttpResponse notFound(std::string body = "Not Found");
    static HttpResponse methodNotAllowed(std::string body = "Method Not Allowed");
    static HttpResponse timeout();
    static HttpResponse conflict(std::string body = "Conflict");
    static HttpResponse tooManyRequests(std::string body = "Too Many Requests");
    static HttpResponse headerTooLarge();
    static HttpResponse bodyTooLarge();
    static HttpResponse internalServerError(std::string body = "Internal Server Error");
    static HttpResponse notImplemented();
    static HttpResponse serviceUnavailable(std::string body = "Service Unavailable");

  private:
    HttpResponse();

    HttpVersion m_version       = HttpVersion::HTTP_1_1;
    HttpStatus  m_status        = HttpStatus::HTTP_OK;
    std::string m_reason_phrase = "OK";

    HeaderMap   m_headers;
    std::string m_body;
};

class HttpResponse::Builder {
  public:
    Builder& setVersion(HttpVersion v) {
        res.m_version = v;
        return *this;
    }

    Builder& setStatus(HttpStatus s) {
        res.m_status        = s;
        res.m_reason_phrase = toReasonPhrase(s);
        return *this;
    }

    Builder& setHeader(const std::string& key, const std::string& value) {
        res.m_headers[key] = value;
        return *this;
    }

    Builder& setBody(const std::string& b) {
        res.m_body = b;
        return *this;
    }

    HttpResponse build() {
        res.m_headers["Content-Length"] = std::to_string(res.m_body.size());
        return res;
    }

  private:
    HttpResponse res;
};