#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <thread>

#include "../utils/threadPool.hpp"
#include "../socket/socket.hpp"
#include "modes/mainMode.hpp"

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

    Server(const ArgParser &parser);                                                                                ///< Constructor
    Server(const ArgParser &parser, std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream); ///< With logger

public:
    static std::string version; ///< Tondik version, if we want to use it in another files

    Logger *l; ///< Logger

    static Server *instance; ///< Singleton

    static Server *init(const ArgParser &parser);                                                                                ///< Factory method with default logger to std out and std err
    static Server *init(const ArgParser &parser, std::ostream &infoStream, std::ostream &errorStream, std::ostream &warnStream); ///< Factory method with logger to own streams

    ~Server(); ///< Destructor

    void start(); ///< Start HTTP Server (create and bind socket + start loop)

    void loop(); ///< Listen loop for incomming connections
};