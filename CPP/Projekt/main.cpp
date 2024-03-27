#include <iostream>
#include <string>
#include <signal.h>
#include <functional>

#include "utils/argParser.hpp"
#include "server/server.hpp"
#include "socket/socket.hpp"

// https://www.geeksforgeeks.org/signals-c-language/ on CTRL + C close socket
void handleSigInt(int s)
{
    if (Server::instance != nullptr)
    {
        Logger::info("Stopping server...");
        Server::instance->~Server();
        Logger::info("Server stopped.");
    }
    exit(s);
}

int main(int argc, char **argv)
{
    signal(SIGINT, handleSigInt);

    ArgParser parser(argc, argv);

    Server s(parser);
    s.start();
}

/*
Socket s("127.0.0.1", 8080);

    StartType type = s.bind();
    if (type != StartType::OK)
    {
        std::cout << type << std::endl;
        return -1;
    }

    std::string str("HTTP/1.1 404 Smri ti koule\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nAhoj");

    while (true)
    {
        auto payload = s.accept();

        if (!payload.has_value())
        {
            std::cout << "skipped" << std::endl;
            continue;
        }

        ReceivedData data = payload.value();

        if ((int)send(data.fd, str.data(), str.length(), 0) == -1)
        {
            continue;
        }

        close(data.fd);

        std::cout << data.data << std::endl;
    };
*/