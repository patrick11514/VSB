#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <thread>

#include "../utils/threadPool.hpp"
#include "modes/devMode.hpp"

/**
 * @brief Main part of HTTP Server
 */
class Server
{
    const ArgParser &parser;  ///< Reference to argument parser
    Socket *socket = nullptr; ///< Pointer on opened socket
    ThreadPool pool;          ///< Thread pool
    MainMode *mode = nullptr;

    void handleRequest(const ReceivedData &data); /// <Function to handle single request

public:
    Logger *l; ///< Logger

    static Server *instance; ///< Singleton

    Server(const ArgParser &parser);                                                                                ///< Constructor
    Server(const ArgParser &parser, std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream); ///< With logger
    ~Server();                                                                                                      ///< Destructor

    void start(); ///< Start HTTP Server (create and bind socket + start loop)

    void loop(); ///< Listen loop for incomming connections
};