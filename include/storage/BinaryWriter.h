#pragma once

#include "./BinaryFormat.h"
#include "VaultData.h"
#include "core/Account.h"
#include "string/SecureString.h"
#include <cstddef>
#include <cstdint>

class BinaryWriter{
private:
    SecureBuffer buffer_;

private:
    void writeBytes(const void*, std::size_t);
    void writeUint32(std::uint32_t);
    void writeSecureString(const SecureString&);
    void writeAccount(const Account&);

public:
    void writeVault(const VaultData&);
    SecureBuffer takeBuffer();
};