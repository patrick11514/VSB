#pragma once

#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class Server
{
    int port = 8080;

public:
    Server() = default;
    Server(int port);

    void start();
};