#include "utils/Security.h"
#include "string/SecureString.h"

#include <sstream>
#include <iomanip>
#include <argon2.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

// === === public === ===
std::array<uint8_t, 32> Security::sha256(
    const uint8_t* data,
    size_t len
) {
    std::array<uint8_t, 32> hash{};

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) return hash;

    do {
        if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1)
            break;

        if (EVP_DigestUpdate(ctx, data, len) != 1)
            break;

        unsigned int outLen = 0;

        if (EVP_DigestFinal_ex(ctx, hash.data(), &outLen) != 1)
            break;

    } while (false);

    EVP_MD_CTX_free(ctx);
    return hash;
}

std::vector<uint8_t> Security::generateSalt(size_t length) {
    if (length == 0) {
        return {};
    }
    
    std::vector<uint8_t> salt(length);
    if(RAND_bytes(salt.data(), static_cast<int>(salt.size())) != 1) {
        throw std::runtime_error("Failed to generate random salt using OpenSSL RAND_bytes");
    }
    return salt;
}

std::array<uint8_t, 32> Security::deriveKey(
    const SecureString& password, 
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
