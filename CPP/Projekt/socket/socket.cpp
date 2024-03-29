#include "socket.hpp"

std::string Socket::getAddress(sockaddr *address)
{
    char data[40];

    // https://gist.github.com/jkomyno/45bee6e79451453c7bbdc22d033a282e
    switch (address->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET, &(((sockaddr_in *)address)->sin_addr), data, 40);
        break;

    case AF_INET6:
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)address)->sin6_addr), data, 40);
        break;
    }

    return std::string(data);
}

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

    // https://stackoverflow.com/a/35419032
    // reuse if in TIME_WAIT
    // ❯ sudo netstat -ntp | grep "8080"
    // tcp        0      0 10.10.10.210:8080       10.10.10.210:54894      TIME_WAIT   -
    int option = 1;
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1)
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

    if (listen(this->fd, this->numberOfConnectionsToAcceptBeforeDrop) == -1)
    {
        return StartType::FAILED_TO_LISTEN;
    }

    return StartType::OK;
}

std::optional<ReceivedData> Socket::accept()
{
    if (this->fd == -1)
    {
        return std::nullopt;
    }

    sockaddr address;
    socklen_t len = sizeof(address);

    int client = ::accept(this->fd, &address, &len);

    if (client == -1)
    {
        return std::nullopt;
    }

    char buffer[1024] = {0};

    if ((int)recv(client, &buffer, sizeof(buffer), 0) == -1)
    {
        throw std::runtime_error("Unable to read input");
    }
    return ReceivedData{client, std::string(buffer), this->getAddress(&address)};
}
