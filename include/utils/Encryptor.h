#pragma once

#include "storage/BinaryFormat.h"
#include <array>
#include <cstdint>

struct EncryptResult {
    SecureBuffer ciphertext;
    std::array<uint8_t, BinaryFormat::NONCE_SIZE> nonce;
    std::array<uint8_t, BinaryFormat::TAG_SIZE> tag;
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
        const std::array<uint8_t, BinaryFormat::NONCE_SIZE>& nonce,
        const std::array<uint8_t, BinaryFormat::TAG_SIZE>& tag
    );

private:
    static void generateRandomNonce(std::array<uint8_t, BinaryFormat::NONCE_SIZE>& nonce);
};
