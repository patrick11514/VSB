#include <iostream>
#include <string>
#include <signal.h>
#include <functional>

#include <fstream>

#include "utils/argParser.hpp"
#include "server/server.hpp"
#include "socket/socket.hpp"
#include "utils/iniParser.hpp"

// https://www.geeksforgeeks.org/signals-c-language/ on CTRL + C close socket
void handleSigInt(int s)
{
    if (Server::instance != nullptr)
    {
        Logger l;
        l.info("Stopping server...");
        delete Server::instance;
        l.info("Server stopped.");
    }
    exit(s);
}

/**
 * In file file://./server/modes/devMode.cpp line 22 we send data to pipe, but if data was transfering
 * and the client terminate request, send return -1 and crashes program on 141 (Broken PIPE), so this
 * function prevents the crash and if statment catches this and returns from function and connection is
 * closed.
 */
void handleBrokenPipe(int) {}

int main(int argc, char **argv)
{

    signal(SIGINT, handleSigInt);
    signal(SIGPIPE, handleBrokenPipe);

    ArgParser parser(argc, argv);

    if (parser.includes("help"))
    {
        std::cout << "Tondík Web Server\n\n";
        std::cout << "Argument types:\n";
        std::cout << " --name=value\n";
        std::cout << " -name value\n\n";
        std::cout << "Argument list:\n";
        std::cout << " --help - shows this help\n";
        std::cout << "Dev mode:\n";
        std::cout << " --dev - start server in dev mode\n";
        std::cout << " --host - start dev server on 0.0.0.0 interface\n";
        std::cout << " --port=8080 - start dev server on specific port\n";
        std::cout << " --path=/path/to/folder - server files in specified folder\n";
        std::cout << "Server mode:\n";
        std::cout << " --path=/etc/tondik - path to config files\n";

        return 0;
    }

    IniParser config("../_testing/example.ini");

    std::ofstream log("/tmp/log.txt");
    std::ofstream err("/tmp/err.txt");
    std::ofstream warn("/tmp/warn.txt");
    Server *s = nullptr;
    try
    {
        s = Server::init(parser /*, log, err, warn*/);
        s->start();
    }
    catch (std::exception &)
    {
    }

    delete s;
}