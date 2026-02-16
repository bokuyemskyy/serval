#pragma once
#include "../../server/include/http_request.hpp"
#include "http_response.hpp"
#include "http_server_config.hpp"

#include <string>

class HttpProtocol {
public:
    HttpProtocol(HttpServerConfig& config);
    HttpRequest parseRequest(const std::string& raw_request);
    std::string serializeResponse(const HttpResponse& response);
    bool         isRequestComplete(const std::string& raw_request);
private:
    HttpServerConfig& m_config;
};