#include "./../../include/storage/BinaryFormat.h"
#include "./../../include/storage/BinaryWriter.h"

#include <cstring>

void BinaryWriter::writeBytes(const void* ptr, size_t size) {
    if(size <= 0) {
        return;
    }

    const auto oldSize = buffer_.size();
    buffer_.resize(oldSize + size);
    std::memcpy(buffer_.data() + oldSize, ptr, size);
}

void BinaryWriter::writeUint32(uint32_t value) {
    uint8_t bytes[4];
    for(int i=0; i<4; ++i) {
        bytes[i] = static_cast<uint8_t>((value >> (i*8)) & 0xFF);
    }

    writeBytes(bytes, sizeof(bytes));
}

void BinaryWriter::writeSecureString(const SecureString& str) {
    writeUint32(static_cast<uint32_t>(str.size()));
    writeBytes(str.data(), str.size());
}

void BinaryWriter::writeAccount(const Account& a) {
    writeSecureString(a.getService());
    writeSecureString(a.getUsername());
    writeSecureString(a.getPassword());
    writeSecureString(a.getCategory());
    writeSecureString(a.getNote());
}

void BinaryWriter::writeVault(const VaultData& v) {
    writeUint32(BinaryFormat::MAGIC);
    writeUint32(BinaryFormat::VERSION);

    writeUint32(static_cast<uint32_t>(v.categories.size()));
    for(const SecureString& cat : v.categories) {
        writeSecureString(cat);
    }

    writeUint32(static_cast<uint32_t>(v.accounts.size()));
    for(const Account& a : v.accounts) {
        writeAccount(a);
    }
}

SecureBuffer BinaryWriter::takeBuffer() {
    return std::move(buffer_);
}
