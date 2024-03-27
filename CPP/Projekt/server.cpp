#include "server.hpp"

Server::Server(const ArgParser &parser) : parser(parser) {}

Server::~Server()
{
    delete this->socket;
}

void Server::start()
{
    // load port from cmd line
    auto portStr = this->parser.getByKey("port");

    int port = 8080;

    if (portStr != nullptr)
    {
        try
        {
            port = std::stoi(*portStr);
            if (port < 1 || port > 65353)
            {
                std::cout << "Invalid port in range 1-65353" << std::endl;
                return;
            }
        }
        catch (const std::exception &)
        {
            std::cout << "Invalid port number: " << portStr << std::endl;
            return;
        }
    }

    // modes
    if (this->parser.includes("dev"))
    {

        bool host = this->parser.includes("host");

        this->socket = new Socket(host == true ? "0.0.0.0" : "127.0.0.1", port);
        StartType result = this->socket->bind();
        if (result != StartType::OK)
        {
            std::cout << "Failed to start server on port " << port << "." << std::endl;
            return;
        }
    }
    else
    {
        // TODO
        std::cout << "Server mode WIP" << std::endl;
    }

    this->loop();
}

void Server::loop() const
{
    std::string str("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nAhoj");
    // todo
    while (true)
    {
        auto data = this->socket->accept();
        if (!data.has_value())
        {
            continue;
        }

        auto clientData = data.value();

        std::cout << "Got data:\n"
                  << clientData.data << std::endl;

        send(clientData.fd, str.data(), str.length(), 0);
        close(clientData.fd);
    }
}
