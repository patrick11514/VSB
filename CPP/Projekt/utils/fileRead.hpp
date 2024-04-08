#pragma once

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

/**
 * @brief Get information about file
 */
class FileRead
{
    static std::unordered_map<fs::path, bool> textTypes; ///< list mime types that are text based

    const fs::path &path;     ///< path to the file
    const fs::path fileName;  ///< file name
    const fs::path extension; ///< file's extension

    bool statusCached = false; ///< if file status is cached
    fs::file_status status;    ///< file status

public:
    FileRead(const fs::path &path); ///< constructor

    bool exists() const;                    ///< check if file exists
    const fs::path &getExtension() const;   ///< get file extension
    const std::string &getMimeType() const; ///< get file's mime type
    bool isBinary() const;                  ///< check if file is binary based of extension

    bool isFolder();                        ///< check if path is folder
    const fs::file_status &getFileStatus(); ///< get file status
};