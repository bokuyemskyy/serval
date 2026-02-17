#pragma once

#include "ihttp_request_handler.hpp"

#include <filesystem>
#include <fstream>
#include <unordered_map>

class StaticFileHandler : public IHttpRequestHandler {
  private:
    std::filesystem::path                        m_root_dir;
    std::unordered_map<std::string, std::string> m_mime_types = {
        {".html", "text/html"},        {".css", "text/css"},  {".js", "application/javascript"},
        {".json", "application/json"}, {".png", "image/png"}, {".jpg", "image/jpeg"},
        {".txt", "text/plain"},
    };

    std::string getMimeType(const std::string& ext) {
        auto it = m_mime_types.find(ext);
        return it != m_mime_types.end() ? it->second : "application/octet-stream";
    }

  public:
    explicit StaticFileHandler(HttpServerConfig config) {
        m_root_dir = std::filesystem::canonical(config.root_directory);

        for (const auto& [key, value] : config.custom_mime_types) {
            m_mime_types[key] = value; // Overwrites existing ones
        }
    }

    HttpResponse handleRequest(const HttpRequest& request) override {
        // Reject invalid paths immediately
        if (request.path.empty() || request.path[0] != '/') {
            return HttpResponse::badRequest();
        }

        std::filesystem::path file_path;
        try {
            file_path = std::filesystem::canonical(m_root_dir / request.path.substr(1));
        } catch (...) {
            return HttpResponse::notFound(); // Path does not exist or is invalid
        }

        // Ensure the resolved path is still inside root_dir
        auto [root_end, _] = std::mismatch(m_root_dir.begin(), m_root_dir.end(), file_path.begin());
        if (root_end != m_root_dir.end()) {
            return HttpResponse::forbidden();
        }

        // Must be a regular file (no symlinks, no dirs)
        if (!std::filesystem::is_regular_file(file_path)) {
            return HttpResponse::notFound();
        }

        // Read the file
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return HttpResponse::internalServerError();
        }

        std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string mime = getMimeType(file_path.extension().string());

        return HttpResponse::ok(body, mime);
    }
};