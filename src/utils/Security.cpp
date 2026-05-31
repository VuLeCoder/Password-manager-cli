#include "./../../include/utils/Security.h"

#include <functional>
#include <sstream>
#include <iomanip>
#include <argon2.h>
#include <random>

// === === public === ===
std::vector<uint8_t> Security::generateSalt(size_t length) {
    std::random_device rd;
    std::vector<uint8_t> salt(length);
    for(size_t i=0; i<length; ++i) {
        salt[i] = static_cast<uint8_t>(rd());
    }
    return salt;
}

std::array<uint8_t, 32> Security::deriveKey(
    const std::string& password, 
    const std::vector<uint8_t>& salt
) {
    std::array<uint8_t, 32> key;

    int res = argon2id_hash_raw(
        V1, V2, V3,
        password.data(), password.size(),
        salt.data(), salt.size(),
        key.data(), key.size()
    );

    if(res != ARGON2_OK) {
        throw std::runtime_error(
            argon2_error_message(res)
        );
    }
    return key;
}

std::string Security::bytesToHex(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;

    for(uint8_t b : bytes) {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(b);
    }

    return ss.str();
}

std::vector<uint8_t> Security::hexToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;

    for(size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);

        bytes.push_back(
            static_cast<uint8_t>(
                std::stoi(byteStr, nullptr, 16)
            )
        );
    }

    return bytes;
}
