#pragma once
#include <string>

class Security {
public:
    static std::string hash(const std::string& password);
    static std::string toHex(const std::string& data);
    static std::string hexToStr(const std::string& hex);

};
