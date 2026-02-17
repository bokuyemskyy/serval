#pragma once

#include <string>
#include <unordered_map>

class FileServerConfig {
  public:
    std::string root_directory = ".";

    bool directory_listing = true;
    bool show_hidden_files = false;

    std::unordered_map<std::string, std::string> custom_mime_types{};

    class Builder;
};

class FileServerConfig::Builder {
  public:
    Builder& setRootDirectory(const std::string& d) {
        m_config.root_directory = d;
        return *this;
    }

    Builder& enableDirectoryListing(bool enabled = true) {
        m_config.directory_listing = enabled;
        return *this;
    }

    Builder& enableShowHiddenFiles(bool enabled = true) {
        m_config.show_hidden_files = enabled;
        return *this;
    }

    Builder& addCustomMimeType(const std::string& key, const std::string& value) {
        m_config.custom_mime_types[key] = value;
        return *this;
    }

    Builder& setCustomMimeTypes(const std::unordered_map<std::string, std::string>& types) {
        m_config.custom_mime_types = types;
        return *this;
    }

    FileServerConfig build() { return m_config; }

  private:
    FileServerConfig m_config;
};
