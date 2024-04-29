#include <fstream>

#include "mainMode.hpp"

MainMode::MainMode(const ArgParser &parser, Logger *logger) : parser(parser), logger(logger) {}

MainMode::~MainMode() {}

void MainMode::doFile(const fs::path &filePath, const HTTPPayload &payload, HTTPResponse &response, const FileRead &file, int fd) const
{
    uintmax_t fileSize = fs::file_size(filePath);

    std::ifstream iFile(filePath);
    // 1024 * 1024
    char data[1048576];

    bool startSet = false;
    size_t start = 0;
    size_t end = fileSize;

    // if client asks for specific range (sadly works only in VLC), browsers
    // ask this through same connection and because we don't support keepalive
    // connection, browsers doesn't support jumping into unbuffered part of vieo
    if (payload.headers.find("Range") != payload.headers.end())
    {
        std::string_view range = payload.headers.at("Range");

        if (!range.starts_with("bytes="))
        {
            // bad request
            response.code = 400;
            return;
        }

        size_t eq = range.find('=');
        std::string_view data(range.begin() + eq + 1, range.end());

        size_t dash = data.find('-');
        if (dash == std::string_view::npos)
        {
            // bad request
            response.code = 400;
            response.send(fd);
            return;
        }

        std::string_view from(data.begin(), data.begin() + dash);
        std::string_view to(data.begin() + dash + 1, data.end());

        if (from.size() > 0)
        {
            startSet = true;
            start = static_cast<size_t>(std::stoul(std::string(from)));
        }

        if (to.size() > 0)
        {
            std::string_view to(data.begin() + dash + 1, data.end());
            end = static_cast<size_t>(std::stoul(std::string(to)));
        }

        if (end > fileSize || start > fileSize)
        {
            response.code = 400;
            response.send(fd);
            return;
        }

        // partial content
        response.code = 206;
    }

    // send empty response with correct length
    if (startSet)
    {
        response.headers.emplace("Content-Range",
                                 std::format("bytes {}-{}/{}", std::to_string(start), std::to_string(end), std::to_string(fileSize)));
    }

    response.headers.emplace("Content-Length", std::to_string(end - start));
    response.headers.emplace("Content-Type", file.getMimeType());
    response.send(fd);

    // jump to start
    iFile.seekg(start);
    size_t read = 0;

    while (!(iFile.eof() || iFile.fail()))
    {
        size_t toRead = sizeof(data);

        size_t diff = end - read;
        if (diff < toRead)
        {
            toRead = static_cast<size_t>(diff);
        }

        iFile.read(data, toRead);

        size_t readed = iFile.gcount();

        if (static_cast<int>(::send(fd, data, readed, 0)) == -1)
        {
            // error or user just canceled request
            return;
        }

        read += readed;

        // end of file
        if (read >= static_cast<size_t>(end))
        {
            return;
        }
    }
}

void MainMode::handleRequest(const ReceivedData &client, const HTTPPayload &)
{
    ::close(client.fd);
}
