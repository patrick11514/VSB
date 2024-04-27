#pragma once

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

fs::path decode(const std::string_view &path); ///< decode http encoded path

template <typename T>
using optionalRef = std::optional<std::reference_wrapper<T>>;

template <typename T>
T &extractRef(optionalRef<T> value)
{
    return value.value().get();
}
