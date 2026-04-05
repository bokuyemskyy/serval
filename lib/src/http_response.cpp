#include "http_response.hpp"

HttpResponse::HttpResponse() = default;

std::string HttpResponse::header(const std::string& name) const {
    auto it = m_headers.find(name);
    return it != m_headers.end() ? it->second : "";
}
bool HttpResponse::hasHeader(const std::string& name) const {
    return m_headers.contains(name);
}

HttpVersion HttpResponse::version() const {
    return m_version;
}
HttpStatus HttpResponse::status() const {
    return m_status;
}
const std::string& HttpResponse::reasonPhrase() const {
    return m_reason_phrase;
}
const HeaderMap& HttpResponse::headers() const {
    return m_headers;
}
const std::string& HttpResponse::body() const {
    return m_body;
}

HttpResponse HttpResponse::ok(std::string body, std::string ct) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_OK)
        .setHeader("Content-Type", ct)
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::json(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_OK)
        .setHeader("Content-Type", "application/json")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::html(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_OK)
        .setHeader("Content-Type", "text/html; charset=utf-8")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::created(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_CREATED)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::noContent() {
    return HttpResponse::Builder().setStatus(HttpStatus::HTTP_NO_CONTENT).build();
}
HttpResponse HttpResponse::badRequest(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_BAD_REQUEST)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::unauthorized(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_UNAUTHORIZED)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::forbidden(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_FORBIDDEN)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::notFound(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_NOT_FOUND)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::methodNotAllowed(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_METHOD_NOT_ALLOWED)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::timeout() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_REQUEST_TIMEOUT)
        .setHeader("Content-Type", "text/plain")
        .setBody("Request Timeout")
        .build();
}
HttpResponse HttpResponse::conflict(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_CONFLICT)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::tooManyRequests(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_TOO_MANY_REQUESTS)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::headerTooLarge() {
    return HttpResponse::Builder()
        .setStatus(static_cast<HttpStatus>(431))
        .setHeader("Content-Type", "text/plain")
        .setBody("Request Header Fields Too Large")
        .build();
}
HttpResponse HttpResponse::bodyTooLarge() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_PAYLOAD_TOO_LARGE)
        .setHeader("Content-Type", "text/plain")
        .setBody("Payload Too Large")
        .build();
}
HttpResponse HttpResponse::internalServerError(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_INTERNAL_SERVER_ERROR)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::notImplemented() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_NOT_IMPLEMENTED)
        .setHeader("Content-Type", "text/plain")
        .setBody("Not Implemented")
        .build();
}
HttpResponse HttpResponse::serviceUnavailable(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::HTTP_SERVICE_UNAVAILABLE)
        .setHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}