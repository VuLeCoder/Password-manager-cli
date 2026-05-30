#pragma once
#include <string>

class Vault{
public:
    static bool exists();
    static bool initialize();

    static bool hasLPTVPassword();
    static void setLPTVPassword(const std::string& password);
    static bool verifyLPTV(const std::string& password);
};
