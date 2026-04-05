#pragma once

#include "file_server_config.hpp"
#include "ihttp_request_handler.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class StaticFileHandler : public IHttpRequestHandler {
  private:
    std::vector<Rule>     m_rules;
    std::filesystem::path m_root_directory;

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
    std::string normalizeUrlPath(std::string path) {
        if (path.empty())
            return "/";

        if (path.back() == '/' && path.size() > 1)
            path.pop_back();

        return path;
    }

    PathConfig resolveConfig(std::string_view path) const {
        PathConfig result;

        for (const auto& rule : m_rules) {
            if (matches(path, rule.path)) {
                const auto& cfg = rule.config;

                if (cfg.directory_listing)
                    result.directory_listing = cfg.directory_listing;
                if (cfg.show_hidden_files)
                    result.show_hidden_files = cfg.show_hidden_files;
                if (cfg.allow_origin)
                    result.allow_origin = cfg.allow_origin;
                if (cfg.allow_credentials)
                    result.allow_credentials = cfg.allow_credentials;
                if (cfg.allow_methods)
                    result.allow_methods = cfg.allow_methods;
                if (cfg.allow_headers)
                    result.allow_headers = cfg.allow_headers;
                if (cfg.custom_headers)
                    result.custom_headers = cfg.custom_headers;

                if (cfg.custom_mime_types) {
                    if (!result.custom_mime_types)
                        result.custom_mime_types = *cfg.custom_mime_types;
                    else
                        result.custom_mime_types->insert(cfg.custom_mime_types->begin(), cfg.custom_mime_types->end());
                }
            }
        }

        return result;
    }

    bool matches(std::string_view path, std::string_view rule) const {
        if (!path.starts_with(rule))
            return false;

        if (path.size() == rule.size())
            return true;

        return path[rule.size()] == '/';
    }

    explicit StaticFileHandler(FileServerConfig config) {
        m_root_directory = std::filesystem::canonical(config.root_directory);

        m_rules = std::move(config.rules);
        for (auto& rule : m_rules) {
            rule.path = normalizeUrlPath(rule.path);
        }
    }

    void applyHeaders(HttpResponse::Builder& builder, const PathConfig& config) const {
        if (config.custom_headers) {
            for (const auto& [k, v] : *config.custom_headers) {
                builder.setHeader(k, v);
            }
        }

        if (config.allow_origin && !config.allow_origin->empty()) {
            builder.setHeader("Access-Control-Allow-Origin", (*config.allow_origin)[0]);
        }

        if (config.allow_credentials.value_or(false)) {
            builder.setHeader("Access-Control-Allow-Credentials", "true");
        }

        if (config.allow_methods) {
            std::string methods;
            for (size_t i = 0; i < config.allow_methods->size(); ++i) {
                if (i)
                    methods += ", ";
                methods += (*config.allow_methods)[i];
            }
            builder.setHeader("Access-Control-Allow-Methods", methods);
        }

        if (config.allow_headers) {
            std::string headers;
            for (size_t i = 0; i < config.allow_headers->size(); ++i) {
                if (i)
                    headers += ", ";
                headers += (*config.allow_headers)[i];
            }
            builder.setHeader("Access-Control-Allow-Headers", headers);
        }
    }

    HttpResponse handle(const HttpRequest& request) override {
        if (request.path().empty() || request.path()[0] != '/') {
            return HttpResponse::badRequest();
        }

        std::string request_path = normalizeUrlPath(request.path());
        auto        config       = resolveConfig(request_path);

        std::filesystem::path file_path;
        try {
            file_path = std::filesystem::canonical(std::filesystem::path(m_root_directory) / request_path.substr(1));
        } catch (...) {
            return HttpResponse::notFound();
        }

        auto relative = std::filesystem::relative(file_path, m_root_directory);

        if (relative.empty() || relative.string().starts_with("..")) {
            return HttpResponse::forbidden();
        }

        if (!config.show_hidden_files.value_or(false)) {
            if (file_path.filename().string().starts_with(".")) {
                return HttpResponse::forbidden();
            }
        }

        if (std::filesystem::is_directory(file_path)) {
            if (!config.directory_listing.value_or(false)) {
                return HttpResponse::forbidden();
            }

            std::string body = "<html><body><ul>";

            for (const auto& entry : std::filesystem::directory_iterator(file_path)) {
                std::string name = entry.path().filename().string();

                if (!config.show_hidden_files.value_or(false) && name.starts_with("."))
                    continue;

                body += "<li><a href=\"" + request_path + "/" + name + "\">" + name + "</a></li>";
            }

            body += "</ul></body></html>";

            HttpResponse::Builder builder;
            builder.setStatus(HttpStatus::HTTP_OK).setHeader("Content-Type", "text/html").setBody(body);

            applyHeaders(builder, config);

            return builder.build();
        }

        if (!std::filesystem::is_regular_file(file_path)) {
            return HttpResponse::notFound();
        }

        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return HttpResponse::internalServerError();
        }

        std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string ext  = file_path.extension().string();
        std::string mime = "application/octet-stream";

        if (config.custom_mime_types && config.custom_mime_types->count(ext)) {
            mime = (*config.custom_mime_types)[ext];
        } else {
            mime = getMimeType(ext);
        }

        HttpResponse::Builder builder;
        builder.setStatus(HttpStatus::HTTP_OK).setHeader("Content-Type", mime).setBody(body);

        applyHeaders(builder, config);

        return builder.build();
    }
};