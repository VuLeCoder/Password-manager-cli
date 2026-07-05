#pragma once

#include <string/SecureString.h>
#include <cstdint>
#include <vector>

using SecureBuffer = std::vector<uint8_t, SecureAllocator<uint8_t>>;

namespace BinaryFormat {
    constexpr uint32_t MAGIC = 311;
    constexpr uint32_t VERSION = 1;
}
