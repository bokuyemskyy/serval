#pragma once

#include "http_types.hpp"

#include <map>
#include <string>

struct HttpResponse {
    HttpResponse();

    HttpVersion version      = HttpVersion::HTTP_1_1;
    HttpStatus  status       = HttpStatus::OK;
    std::string reasonPhrase = "OK";

    HeaderMap   headers;
    std::string body;

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
};

class HttpResponse::Builder {
  public:
    Builder& setVersion(HttpVersion v) {
        res.version = v;
        return *this;
    }

    Builder& setStatus(HttpStatus s) {
        res.status       = s;
        res.reasonPhrase = toReasonPhrase(s);
        return *this;
    }

    Builder& addHeader(const std::string& key, const std::string& value) {
        res.headers[key] = value;
        return *this;
    }

    Builder& setBody(const std::string& b) {
        res.body = b;
        addHeader("Content-Length", std::to_string(b.size()));
        return *this;
    }

    HttpResponse build() { return res; }

  private:
    HttpResponse res;
};