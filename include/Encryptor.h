#pragma once
#include <string>

class Encryptor{
public:
    static std::string encrypt(
        const std::string& data,
        const std::string& key
    );

    static std::string decrypt(
        const std::string& data,
        const std::string& key
    );
};
