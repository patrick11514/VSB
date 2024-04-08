#include "serverMode.hpp"
#include "../../utils/iniParser.hpp"

#include <iostream>
#include <fstream>
#include <format>

ServerMode::ServerMode(const ArgParser &parser, Logger &logger, const std::string &configPath) : MainMode(parser, logger)
{
    fs::path path(fs::canonical(configPath));

    fs::create_directory(path);

    fs::path mainPath(path);
    mainPath.concat("/main.ini");

    if (!fs::exists(mainPath))
    {
        std::ofstream configFile(mainPath);
        // configFile << std::format("port=80\nconfigs_folder={}\n\naccess_log={}\nerror_log={}", path, path.concat);
    }

    // IniParser config(mainPath);

    fs::path p1("/home/patrick115");
    fs::path p2("/home/patrick115/");

    std::cout << p1 / "neco" << std::endl;
    std::cout << p1 / "/neco" << std::endl;
    std::cout << p2 / "neco" << std::endl;
    std::cout << p2 / "/neco" << std::endl;

    // IniParser config(path.concat("/main.ini"));
}

ServerMode::~ServerMode() {}
