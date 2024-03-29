#include "mainMode.hpp"

MainMode::MainMode(const ArgParser &parser, Logger &logger) : parser(parser), logger(logger) {}

MainMode::~MainMode() {}

void MainMode::handleRequest(const ReceivedData &client, const HTTPPayload &data)
{
    ::close(client.fd);
}
