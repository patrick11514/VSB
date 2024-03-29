#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>
#include <optional>

enum StartType
{
    OK,
    FAILED_TO_CREATE_SOCKET, ///< Failed to create socket
    FAILED_TO_BIND,          ///< Failed to bind socket on given address or port
    FAILED_TO_LISTEN
};

struct ReceivedData
{
    int fd;
    std::string data;
    std::string address;
};

class Socket
{
    in_addr_t address = INADDR_ANY;
    int port = 8080;
    int fd = -1;

    std::string getAddress(sockaddr *address);

    size_t numberOfConnectionsToAcceptBeforeDrop = 1000;

public:
    Socket() = default;
    Socket(in_addr_t address, int port);
    Socket(const char *address, int port);
    Socket(Socket &sock);
    ~Socket();

    void close(int s);
    bool isOpened() const;
    StartType bind();
    std::optional<ReceivedData> accept();
};