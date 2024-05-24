#include "mimeType.hpp"
#include <iostream>

std::unordered_map<fs::path, std::string> MimeType::mimeTypes = {
    // text formats
    {".css", "text/css; charset=UTF-8"},
    {".csv", "text/csv; charset=UTF-8"},
    {".html", "text/html; charset=UTF-8"},
    {".htm", "text/html; charset=UTF-8"},
    {".js", "text/javascript; charset=UTF-8"},
    {".json", "application/json; charset=UTF-8"},
    {".mjs", "text/javascript; charset=UTF-8"},
    {".pdf", "application/pdf"},
    {".php", "application/x-httpd-php"},
    {".txt", "text/plain; charset=UTF-8"},
    {".xhtml", "application/xhtml+xml"},
    {".xml", "application/xml"},
    // image fomats
    {".avif", "image/avif"},
    {".bmp", "image/bmp"},
    {".gif", "image/gif"},
    {".ico", "image/vnd.microsoft.icon"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".svg", "image/svg"},
    {".tif", "image/tiff"},
    {".tiff", "image/tiff"},
    {".webp", "image/webp"},
    // video formats
    {".avi", "video/avi"},
    {".mp4", "video/mp4"},
    {".mpeg", "video/mpeg"},
    {".webm", "video/webm"},
    // audio formats
    {".mp3", "audio/mpeg"},
    {".wav", "audio/vaw"},
    {".weba", "audio/webm"},
    // fonts
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {"DEFAULT_TYPE", "application/octet-stream"}

};

const std::string &MimeType::get(const fs::path &extension)
{
    try
    {
        return MimeType::mimeTypes.at(extension);
    }
    catch (const std::out_of_range &)
    {
        return MimeType::mimeTypes["DEFAULT_TYPE"];
    }
}

void MimeType::add(const fs::path &path, const std::string &value)
{
    MimeType::mimeTypes.emplace(path, value);
}
