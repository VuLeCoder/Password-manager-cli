#pragma once
#include <cstdint>
#include <array>

class AuthGuard{
public:
    static bool verify();
    static void unlock(const std::array<uint8_t, 32>& k);
    static void lock();
};
