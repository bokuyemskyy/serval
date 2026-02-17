#pragma once

#include <map>
#include <string>

struct HttpResponse {
  public:
    explicit HttpResponse(int status = 200, std::string body = "", std::string contentType = "text/plain")
        : m_status(status), m_body(std::move(body)) {
        setHeader("Content-Type", std::move(contentType));
    }

    void setHeader(const std::string& key, const std::string& value) { m_headers[key] = value; }

    std::string toString() const {
        std::string response = "HTTP/1.1 " + std::to_string(m_status) + " " + reasonPhrase() + "\r\n";
        for (auto& [key, value] : m_headers) {
            response += key + ": " + value + "\r\n";
        }
        response += "Content-Length: " + std::to_string(m_body.size()) + "\r\n";
        response += "\r\n";
        response += m_body;
        return response;
    }

    static HttpResponse ok(std::string body = "", std::string ct = "text/plain") {
        return HttpResponse(200, std::move(body), std::move(ct));
    }

    static HttpResponse json(std::string body) { return HttpResponse(200, std::move(body), "application/json"); }

    static HttpResponse html(std::string body) {
        return HttpResponse(200, std::move(body), "text/html; charset=utf-8");
    }

    static HttpResponse created(std::string body = "") { return HttpResponse(201, std::move(body), "text/plain"); }

    static HttpResponse noContent() { return HttpResponse(204); }

    static HttpResponse badRequest(std::string body = "Bad Request") {
        return HttpResponse(400, std::move(body), "text/plain");
    }

    static HttpResponse unauthorized(std::string body = "Unauthorized") {
        return HttpResponse(401, std::move(body), "text/plain");
    }

    static HttpResponse forbidden(std::string body = "Forbidden") {
        return HttpResponse(403, std::move(body), "text/plain");
    }

    static HttpResponse notFound(std::string body = "Not Found") {
        return HttpResponse(404, std::move(body), "text/plain");
    }

    static HttpResponse methodNotAllowed(std::string body = "Method Not Allowed") {
        return HttpResponse(405, std::move(body), "text/plain");
    }

    static HttpResponse timeout() { return HttpResponse(408, "Request Timeout", "text/plain"); }

    static HttpResponse conflict(std::string body = "Conflict") {
        return HttpResponse(409, std::move(body), "text/plain");
    }

    static HttpResponse tooManyRequests(std::string body = "Too Many Requests") {
        return HttpResponse(429, std::move(body), "text/plain");
    }

    static HttpResponse headerTooLarge() { return HttpResponse(431, "Request Header Fields Too Large", "text/plain"); }

    static HttpResponse bodyTooLarge() { return HttpResponse(413, "Payload Too Large", "text/plain"); }

    static HttpResponse internalServerError(std::string body = "Internal Server Error") {
        return HttpResponse(500, std::move(body), "text/plain");
    }

    static HttpResponse notImplemented() { return HttpResponse(501, "Not Implemented", "text/plain"); }

    static HttpResponse serviceUnavailable(std::string body = "Service Unavailable") {
        return HttpResponse(503, std::move(body), "text/plain");
    }

  private:
    int                                m_status;
    std::string                        m_body;
    std::map<std::string, std::string> m_headers;

    std::string reasonPhrase() const {
        switch (m_status) {
            case 200:
                return "OK";
            case 408:
                return "Request Timeout";
            case 413:
                return "Payload Too Large";
            case 431:
                return "Request Header Fields Too Large";
            case 500:
                return "Internal Server Error";
            default:
                return "Unknown";
        }
    }
};