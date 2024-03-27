#include <iostream>
#include <string>

#include "argParser.hpp"
#include "server.hpp"

int main(int argc, char **argv)
{
    ArgParser parser(argc, argv);

    Server s;
    s.start();
}
