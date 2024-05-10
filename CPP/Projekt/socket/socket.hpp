#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdexcept>
#include <optional>

/**
 * @brief Start type of socket returned when called bind
 */
enum class StartType
{
    OK,
    FAILED_TO_CREATE_SOCKET, ///< Failed to create socket
    FAILED_TO_BIND,          ///< Failed to bind socket on given address or port
    FAILED_TO_LISTEN
};

/**
 * @brief Structre, that holds Received data from client
 */
struct ReceivedData
{
    int fd;
    std::string data;
    std::string address;
};

/**
 * @brief Class that handles socket binding and accepting connections
 */
class Socket
{
    in_addr_t address = INADDR_ANY; ///< Address on what should be server started
    int port = 8080;                ///< Port on what should be server started
    int fd = -1;                    ///< File descriptor of binded socket

    /**
     * @brief Get ip address from socketaddr
     * @param address address get from client
     * @return ip address as string
     */
    std::string getAddress(sockaddr *address);

    size_t numberOfConnectionsToAcceptBeforeDrop = 1000;

public:
    Socket() = default;                    ///< Default constructor
    Socket(in_addr_t address, int port);   ///< Constructor from addresss and port
    Socket(const char *address, int port); ///< Constructor from string address and port
    Socket(Socket &sock);                  ///< Copy constructor
    ~Socket();                             ///< Destructor

    /**
     * @brief Close binded socket (this->fd) and exit program
     * @param s Code with will be program exitex
     */
    void close(int s);
    bool isOpened() const;                ///< Return true if socket is opened eg. this->fd != -1
    StartType bind();                     ///< Bind socket and return status
    std::optional<ReceivedData> accept(); ///< Accept connection
};