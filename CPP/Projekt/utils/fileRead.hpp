#pragma once

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

class FileRead
{
    static std::unordered_map<fs::path, bool> textTypes;

    const fs::path &path;
    const fs::path fileName;
    const fs::path extension;

    bool statusCached = false;
    fs::file_status status;

    std::string_view fileExtension;

public:
    FileRead(const fs::path &path);

    bool exists() const;
    const fs::path &getExtension() const;
    const std::string &getMimeType() const;
    bool isBinary() const;

    bool isFolder();
    const fs::file_status &getFileStatus();
};