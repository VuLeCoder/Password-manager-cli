#pragma once

#include <vector>
#include <cstdint>

class Encryptor {
public:
    static bool encrypt(
        const std::vector<uint8_t>& plaintext,
        const uint8_t* key,
        const uint8_t* nonce,
        std::vector<uint8_t>& ciphertext,
        std::array<uint8_t, 16>& tag
    );

    static bool decrypt(
        const std::vector<uint8_t>& ciphertext,
        const uint8_t* key,
        const uint8_t* nonce,
        const uint8_t* tag,
        std::vector<uint8_t>& plaintext
    );
};
