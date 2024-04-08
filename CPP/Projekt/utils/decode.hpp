#pragma once

#include <filesystem>

namespace fs = std::filesystem;

fs::path decode(const std::string_view &path); ///< decode http encoded path