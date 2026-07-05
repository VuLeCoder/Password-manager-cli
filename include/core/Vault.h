#pragma once
#include <string/SecureString.h>
#include <cstdint>
#include <array>

class Vault{
public:
    static bool exists();
    static bool initialize();

    static bool hasLPTVPassword();
    static void setLPTVPassword(const SecureString& password);
    static std::pair<bool, std::array<uint8_t, 32>> verifyLPTV(const SecureString& password);
};
