#include "fileRead.hpp"
#include "mimeType.hpp"

std::unordered_map<fs::path, bool> FileRead::textTypes = {
    {".css", true},
    {".csv", true},
    {".html", true},
    {".htm", true},
    {".js", true},
    {".mjs", true},
    {".json", true},
    {".php", true},
    {".txt", true},
    {".xhtml", true},
    {".xml", true}};

FileRead::FileRead(const fs::path &path) : path(path), fileName(path.filename()), extension(fileName.extension()) {}

bool FileRead::exists() const
{
    return fs::exists(this->path);
}

const fs::path &FileRead::getExtension() const
{
    return this->extension;
}

const std::string &FileRead::getMimeType() const
{
    return MimeType::get(this->extension);
}

bool FileRead::isBinary() const
{
    return FileRead::textTypes.find(this->extension) == FileRead::textTypes.end();
}

bool FileRead::isFolder()
{
    return fs::is_directory(this->getFileStatus());
}

const fs::file_status &FileRead::getFileStatus()
{
    if (!this->statusCached)
    {
        this->status = fs::status(this->path);
        this->statusCached = true;
    }

    return this->status;
}
