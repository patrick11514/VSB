#include "server.hpp"

Server *Server::instance = nullptr;

Server::Server(const ArgParser &parser) : parser(parser)
{
    if (Server::instance != nullptr)
    {
        this->l.info("You can only create single instance of server");
        this->~Server();
        return;
    }
    Server::instance = this;
}

Server::Server(const ArgParser &parser, std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream) : parser(parser), l(infoStream, errorStream, warnStream)
{
}

Server::~Server()
{
    delete this->socket;
}

void Server::start()
{
    // load port from cmd line
    auto portStr = this->parser.getByKey("port");

    int port = 8080;

    // if port is presented in cmd line args
    if (portStr != nullptr)
    {
        try
        {
            port = std::stoi(*portStr);
            if (port < 1 || port > 65353)
            {
                this->l.error("Invalid port in range 1-65353");
                return;
            }
        }
        catch (const std::exception &)
        {
            this->l.error("Invalid port number: " + *portStr);
            return;
        }
    }

    // modes
    if (this->parser.includes("dev"))
    {

        bool host = this->parser.includes("host");

        if (host == true)
        {
            this->socket = new Socket(INADDR_ANY, port);
        }
        else
        {
            this->socket = new Socket("127.0.0.1", port);
        }

        StartType result = this->socket->bind();
        if (result != StartType::OK)
        {
            this->l.error("Failed to start server on port " + std::to_string(port) + ".");
            return;
        }

        this->l.info("DEV server started!");
        this->l.info(std::format("Address: http://{}:{}", (host == true ? "0.0.0.0" : "127.0.0.1"), port));
    }
    else
    {
        // TODO
        this->l.warn("Server mode WIP");
        return;
    }

    this->loop();
}

void someLoop()
{
    for (int i = 0; i < 900000000; ++i)
    {
    }
}

void Server::loop()
{
    std::string str("HTTP/1.1 666\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nAhoj");

    while (true)
    {
        // accept connection
        auto data = this->socket->accept();
        // if accepting failed
        if (!data.has_value())
        {
            continue;
        }

        // get client data
        auto clientData = data.value();

        this->pool.push(
            [clientData = std::move(clientData), this]
            {
                this->handleRequest(clientData);
            });
    }
}

void Server::handleRequest(const ReceivedData &data)
{
    // parse HTTP Payload
    HTTPPayload payload(data);

    if (!payload.isValid)
    {
        close(data.fd);
        return;
    }

    this->l.info(std::format("{} to {} from {}", payload.method, payload.path, data.address));

    // someLoop();
    // 96806 requests in 30.10s,

    HTTPResponse response(std::string(payload.httpVersion), 200);
    response.content = "<!DOCTYPE HTML><html><head><title>Super CPP</title></head><body><h1 style=\"color:red;\">CPP je super!!</h1></body></html>";
    response.headers.emplace("Content-Type", "text/html");
    response.send(data.fd);

    // send(clientData.fd, payload.content.data(), payload.content.size(), 0);
    // send(clientData.fd, str.data(), str.size(), 0);
    // close connection with client
    close(data.fd);
}
