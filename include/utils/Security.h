#pragma once
#include <string/SecureString.h>
#include <cstdint>
#include <array>
#include <string>
#include <vector>

class Security {
private:
    inline constexpr static int V1 = 3; 
    inline constexpr static int V2 = 65536; 
    inline constexpr static int V3 = 1;

public:
    static std::array<uint8_t, 32> sha256(
        const uint8_t* data,
        size_t len
    );

    static std::vector<uint8_t> generateSalt(size_t length = 16);

    static std::array<uint8_t, 32> deriveKey(
        const SecureString& password,
        const std::vector<uint8_t>& salt
    );

    static std::string bytesToHex(const std::vector<uint8_t>& bytes);

    static std::vector<uint8_t> hexToBytes(const std::string& hex);
};
