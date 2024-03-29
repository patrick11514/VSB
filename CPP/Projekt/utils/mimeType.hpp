#pragma once

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

class MimeType
{
    static std::unordered_map<fs::path, std::string> mimeTypes;

public:
    static const std::string &get(const fs::path &extension);
};