#pragma once

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

/**
 * @brief List of mime types
 * @link https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
 */
class MimeType
{
    static std::unordered_map<fs::path, std::string> mimeTypes; ///< Mime types map extension -> mime type

public:
    static const std::string &get(const fs::path &extension); ///< get mime type based of extension
};