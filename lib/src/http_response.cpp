#include "http_response.hpp"

HttpResponse::HttpResponse() = default;

std::string HttpResponse::header(const std::string& name) const {
    auto it = headers.find(name);
    return it != headers.end() ? it->second : "";
}
bool HttpResponse::hasHeader(const std::string& name) const {
    return headers.contains(name);
}

HttpResponse HttpResponse::ok(std::string body, std::string ct) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::OK)
        .addHeader("Content-Type", ct)
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::json(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::OK)
        .addHeader("Content-Type", "application/json")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::html(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::OK)
        .addHeader("Content-Type", "text/html; charset=utf-8")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::created(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::CREATED)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::noContent() {
    return HttpResponse::Builder().setStatus(HttpStatus::NO_CONTENT).build();
}
HttpResponse HttpResponse::badRequest(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::BAD_REQUEST)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::unauthorized(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::UNAUTHORIZED)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::forbidden(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::FORBIDDEN)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::notFound(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::NOT_FOUND)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::methodNotAllowed(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::METHOD_NOT_ALLOWED)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::timeout() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::REQUEST_TIMEOUT)
        .addHeader("Content-Type", "text/plain")
        .setBody("Request Timeout")
        .build();
}
HttpResponse HttpResponse::conflict(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::CONFLICT)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::tooManyRequests(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::TOO_MANY_REQUESTS)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::headerTooLarge() {
    return HttpResponse::Builder()
        .setStatus(static_cast<HttpStatus>(431))
        .addHeader("Content-Type", "text/plain")
        .setBody("Request Header Fields Too Large")
        .build();
}
HttpResponse HttpResponse::bodyTooLarge() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::PAYLOAD_TOO_LARGE)
        .addHeader("Content-Type", "text/plain")
        .setBody("Payload Too Large")
        .build();
}
HttpResponse HttpResponse::internalServerError(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::INTERNAL_SERVER_ERROR)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}
HttpResponse HttpResponse::notImplemented() {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::NOT_IMPLEMENTED)
        .addHeader("Content-Type", "text/plain")
        .setBody("Not Implemented")
        .build();
}
HttpResponse HttpResponse::serviceUnavailable(std::string body) {
    return HttpResponse::Builder()
        .setStatus(HttpStatus::SERVICE_UNAVAILABLE)
        .addHeader("Content-Type", "text/plain")
        .setBody(std::move(body))
        .build();
}