#include "server.hpp"

Server *Server::instance = nullptr;

Server::Server(const ArgParser &parser) : parser(parser)
{
    this->l = new Logger();

    if (Server::instance != nullptr)
    {
        this->l->info("You can only create single instance of server");
        this->~Server();
        return;
    }
    Server::instance = this;
}

Server::Server(const ArgParser &parser, std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream)
    : parser(parser)
{
    this->l = new Logger(infoStream, errorStream, warnStream);
}

Server::~Server()
{
    delete this->mode;
    delete this->l;
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
                this->l->error("Invalid port in range 1-65353");
                return;
            }
        }
        catch (const std::exception &)
        {
            this->l->error("Invalid port number: " + *portStr);
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
            this->l->error("Failed to start server on port " + std::to_string(port) + ".");
            return;
        }

        this->mode = new DevMode(parser, *this->l);

        this->l->info("Tondík DEV server started!");
        this->l->info(std::format("Address: http://{}:{}", (host == true ? "0.0.0.0" : "127.0.0.1"), port));
    }
    else
    {
        // TODO
        this->l->warn("Server mode WIP");
        return;
    }

    this->loop();
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

        // push task to ThreadPool
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

    this->mode->handleRequest(data, payload);
}
