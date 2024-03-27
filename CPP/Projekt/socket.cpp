#include "socket.hpp"
#include <iostream>

Socket::Socket(in_addr_t address, int port) : address(address), port(port) {}

Socket::Socket(const char *address, int port) : Socket(inet_addr(address), port) {}

Socket::Socket(Socket &sock)
{
    sock.address = this->address;
    sock.port = this->port;
}

Socket::~Socket()
{
    if (this->fd > -1)
    {
        ::close(this->fd);
    }
}

void Socket::close(int s)
{
    if (this->fd > -1)
    {
        ::close(this->fd);
    }
    exit(s);
}

bool Socket::isOpened() const
{
    return this->fd > -1;
}

StartType Socket::bind()
{
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd == -1)
    {
        return StartType::FAILED_TO_CREATE_SOCKET;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(this->port);
    address.sin_addr.s_addr = this->address;

    if (::bind(this->fd, (sockaddr *)&address, sizeof(address)) == -1)
    {
        return StartType::FAILED_TO_BIND;
    }

    if (listen(this->fd, 2) == -1)
    {
        return StartType::FAILED_TO_LISTEN;
    }

    return StartType::OK;
}

std::optional<ReceivedData> Socket::accept()
{
    int client = ::accept(this->fd, nullptr, nullptr);

    if (client == -1)
    {
        return std::nullopt;
    }

    char buffer[1024] = {0};

    if ((int)recv(client, &buffer, sizeof(buffer), 0) == -1)
    {
        throw std::runtime_error("Unable to read input");
    }
    return ReceivedData{client, std::string(buffer)};
}
