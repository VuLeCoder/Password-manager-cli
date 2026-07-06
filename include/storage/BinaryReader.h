#pragma once

#include "BinaryFormat.h"
#include "core/Account.h"
#include "VaultData.h"
#include <array>

struct RawFile{
    std::array<uint8_t, BinaryFormat::NONCE_SIZE> nonce;
    std::array<uint8_t, BinaryFormat::TAG_SIZE> tag;
    SecureBuffer ciphertext;
};

class BinaryReader{
private:
    const uint8_t* data_;
    std::size_t size_;
    std::size_t cursor_;

private:
    void require(std::size_t n);
    uint32_t readUint32();
    SecureString readSecureString();
    Account readAccount();

public:
    explicit BinaryReader(const SecureBuffer&);
    RawFile readHeader();
    VaultData readVault();
};
