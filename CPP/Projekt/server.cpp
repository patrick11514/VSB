#include "server.hpp"
#include <unistd.h>

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
    address.sin_addr.s_addr = INADDR_ANY;

    bind(soc, (sockaddr *)&address, sizeof(address));
    listen(soc, 5);

    while (true)
    {
        int client = accept(soc, nullptr, nullptr);

        char buffer[1024] = {0};
        recv(client, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer
                  << std::endl;
    }

    // closing the socket.
    close(soc);
}
