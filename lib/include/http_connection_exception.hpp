#pragma once

#include <exception>
#include <string>

class HttpConnectionException : public std::exception {
  public:
    explicit HttpConnectionException(const std::string& msg) : m_message(msg) {}
    const char* what() const noexcept override { return m_message.c_str(); }

  private:
    std::string m_message;
};

class IncompleteRequestException : public HttpConnectionException {
  public:
    IncompleteRequestException() : HttpConnectionException("Connection closed before full HTTP request was received") {}
};

class MalformedRequestException : public HttpConnectionException {
  public:
    MalformedRequestException() : HttpConnectionException("HTTP request is malformed") {}
};

class RequestTimeoutException : public HttpConnectionException {
  public:
    RequestTimeoutException() : HttpConnectionException("HTTP request timed out") {}
};

class RequestHeaderTooLargeException : public HttpConnectionException {
  public:
    RequestHeaderTooLargeException() : HttpConnectionException("HTTP headers exceed maximum allowed size") {}
};

class RequestBodyTooLargeException : public HttpConnectionException {
  public:
    RequestBodyTooLargeException() : HttpConnectionException("HTTP body exceeds maximum allowed size") {}
};

class WouldBlockException : public HttpConnectionException {
  public:
    explicit WouldBlockException()
        : HttpConnectionException("Would block received even though epoll signaled readiness") {}
};