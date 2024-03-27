#pragma once

#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "argParser.hpp"
#include "socket.hpp"

class Server
{
    const ArgParser &parser;
    Socket *socket = nullptr;

public:
    Server(const ArgParser &parser);
    ~Server();

    void start();

    void loop() const;
};