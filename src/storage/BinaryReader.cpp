#include "storage/BinaryReader.h"
#include "exception/StorageException.h"
#include "storage/BinaryFormat.h"

void BinaryReader::require(std::size_t n) {
    if(n > size_ - cursor_) {
        throw StorageException(
            StorageCode::InvalidFormat,
            "Missing something"
        );
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
    if (length == 0) {
        return SecureString();
    }
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

RawFile BinaryReader::readHeader() {
    uint32_t magic = readUint32();
    if(magic != BinaryFormat::MAGIC) {
        throw StorageException(
            StorageCode::Unknown,
            "Invalid magic number"
        );
    }

    uint32_t version = readUint32();
    if(version != BinaryFormat::VERSION) {
        throw StorageException(
            StorageCode::UnsupportedVersion,
            "Different versions"
        );
    }

    RawFile rFile;
    
    require(BinaryFormat::NONCE_SIZE);
    std::copy(data_ + cursor_, data_ + cursor_ + BinaryFormat::NONCE_SIZE, rFile.nonce.begin());
    cursor_ += BinaryFormat::NONCE_SIZE;

    require(BinaryFormat::TAG_SIZE);
    std::copy(data_ + cursor_, data_ + cursor_ + BinaryFormat::TAG_SIZE, rFile.tag.begin());
    cursor_ += BinaryFormat::TAG_SIZE;

    rFile.ciphertext.assign(
        data_ + cursor_,
        data_ + size_
    );

    return rFile;
}

VaultData BinaryReader::readVault() {
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
