#pragma once

#include <string>
#include <map>

class HttpResponse {
public:
    explicit HttpResponse(int status_code = 200, const std::string& body = "")
        : m_status(status_code), m_body(body) {}

    void setHeader(const std::string& key, const std::string& value) {
        m_headers[key] = value;
    }

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

    static HttpResponse timeout() {
        HttpResponse resp(408, "Request Timeout");
        resp.setHeader("Content-Type", "text/plain");
        return resp;
    }

    static HttpResponse headerTooLarge() {
        HttpResponse resp(431, "Request Header Fields Too Large");
        resp.setHeader("Content-Type", "text/plain");
        return resp;
    }

    static HttpResponse bodyTooLarge() {
        HttpResponse resp(413, "Payload Too Large");
        resp.setHeader("Content-Type", "text/plain");
        return resp;
    }

    static HttpResponse internalServerError() {
        HttpResponse resp(500, "Internal Server Error");
        resp.setHeader("Content-Type", "text/plain");
        return resp;
    }

private:
    int m_status;
    std::string m_body;
    std::map<std::string, std::string> m_headers;

    std::string reasonPhrase() const {
        switch (m_status) {
            case 200: return "OK";
            case 408: return "Request Timeout";
            case 413: return "Payload Too Large";
            case 431: return "Request Header Fields Too Large";
            case 500: return "Internal Server Error";
            default: return "Unknown";
        }
    }
};