#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Engine {
using ByteVector = std::vector<std::byte>;

std::string readFile(std::string_view path);
} // namespace Engine