#include "server.hpp"

Server::Server(int port) : port(port)
{
}

void Server::start()
{
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == -1)
    {
        throw std::runtime_error("Unable to open socket");
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("0.0.0.0");

    std::cout << "bind: " << bind(soc, (sockaddr *)&address, sizeof(address)) << std::endl;
    std::cout << "listen: " << listen(soc, 5) << std::endl;

    while (true)
    {
        int client = accept(soc, nullptr, nullptr);

        char buffer[1024] = {0};
        ssize_t aa = recv(client, buffer, sizeof(buffer), 0);
        std::cout << aa << std::endl;
        std::cout << "Message from client: " << buffer
                  << std::endl;
    }

    // closing the socket.
    close(soc);
}
