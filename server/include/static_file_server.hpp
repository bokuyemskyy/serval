#pragma once

#include "../../lib/include/ihttp_request_handler.hpp"

class StaticFileHandler : public IHttpRequestHandler {
    std::filesystem::path root_dir;

public:
    explicit StaticFileHandler(std::filesystem::path root) : root_dir(root) {}

    HttpResponse handleRequest(const HttpRequest& request) override {
        // auto file_path = root_dir / request.path;
        // Security checks, file reading, mime types, etc.
        // return HttpResponse::from_file(file_path);
        return  HttpResponse::headerTooLarge();
    }
};