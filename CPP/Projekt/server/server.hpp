#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <thread>

#include "../utils/argParser.hpp"
#include "../socket/socket.hpp"
#include "../utils/logger.hpp"
#include "payload.hpp"
#include "response.hpp"

/**
 * @brief Main part of HTTP Server
 */
class Server
{
    const ArgParser &parser;  ///< Reference to argument parser
    Socket *socket = nullptr; ///< Pointer on opened socket

    void handleRequest(const ReceivedData &data) const;

public:
    static Server *instance; ///< Instance of server, because if we CTRL-C program, we want to access socket and close it

    Server(const ArgParser &parser); ///< Constructor
    ~Server();                       ///< Destructor

    void start(); ///< Start HTTP Server (create and bind socket + start loop)

    void loop() const; ///< Listen loop for incomming connections
};