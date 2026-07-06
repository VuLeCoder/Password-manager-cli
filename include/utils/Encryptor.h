#pragma once

#include "storage/BinaryFormat.h"

#include <array>
#include <cstdint>

struct EncryptResult {
    SecureBuffer ciphertext;
    std::array<uint8_t, 12> nonce;
    std::array<uint8_t, 16> tag;
};

class Encryptor {
public:
    static EncryptResult encrypt(
        const SecureBuffer& plaintext,
        const uint8_t* key
    );

    static SecureBuffer decrypt(
        const SecureBuffer& ciphertext,
        const uint8_t* key,
        const std::array<uint8_t,12>& nonce,
        const std::array<uint8_t,16>& tag
    );

private:
    static void generateRandomNonce(std::array<uint8_t,12>& nonce);
};
