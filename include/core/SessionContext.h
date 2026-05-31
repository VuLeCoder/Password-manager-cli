#pragma once
#include <array>
#include <cstdint>

class SessionContext {
private:
    std::array<uint8_t, 32> key{};
    bool active = false;

public:
    static SessionContext& get();

    void setKey(const std::array<uint8_t, 32>& k);
    const std::array<uint8_t, 32>& getKey() const;

    void clear();
    bool isActive() const;
};
