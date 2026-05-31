#pragma once
#include <string>
#include <cstdint>
#include <array>
#include <utility>

class Vault{
public:
    static bool exists();
    static bool initialize();

    static bool hasLPTVPassword();
    static void setLPTVPassword(const std::string& password);
    static std::pair<bool, std::array<uint8_t, 32>> verifyLPTV(const std::string& password);
};
