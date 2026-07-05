#include "./../../include/storage/BinaryReader.h"

void BinaryReader::require(std::size_t n) {
    if(cursor_ + n > size_) {
    }
}

uint32_t BinaryReader::readUint32() {
    uint32_t value = 0;
    require(sizeof(value));

    for(size_t i=0; i<sizeof(value); ++i) {
        value |= static_cast<uint32_t>(data_[cursor_ + i]) << (i * 8);
    }

    cursor_ += sizeof(value);
    return value;
}

SecureString BinaryReader::readSecureString() {
    uint32_t length = readUint32();
    require(length);

    SecureString str;
    str.assign(
        reinterpret_cast<const char*>(data_ + cursor_),
        length
    );

    cursor_ += length;
    return str;
}

Account BinaryReader::readAccount() {
    Account a;
    a.setService(readSecureString());
    a.setUsername(readSecureString());
    a.setPassword(readSecureString());
    a.setCategory(readSecureString());
    a.setNote(readSecureString());

    return a;
}

BinaryReader::BinaryReader(const SecureBuffer& buffer) {
    data_ = buffer.data();
    size_ = buffer.size();
    cursor_ = 0;
}

VaultData BinaryReader::readVault() {
    uint32_t magic = readUint32();
    if(magic != BinaryFormat::MAGIC) {

    }

    uint32_t version = readUint32();
    if(version != BinaryFormat::VERSION) {

    }

    VaultData vaultData;

    size_t size = readUint32();
    vaultData.categories.reserve(size);
    for(size_t i=0; i<size; ++i) {
        vaultData.categories.push_back(readSecureString());
    }

    size = readUint32();
    vaultData.accounts.reserve(size);
    for(size_t i=0; i<size; ++i) {
        vaultData.accounts.push_back(readAccount());
    }

    return vaultData;
}
