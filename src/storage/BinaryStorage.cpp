#include "core/SessionContext.h"
#include "exception/StorageException.h"
#include "storage/BinaryStorage.h"
#include "storage/BinaryWriter.h"
#include "storage/BinaryReader.h"
#include "storage/BinaryFormat.h"
#include "storage/VaultData.h"
#include "utils/Encryptor.h"

#include <fstream>

// === === === === ===
// private:
// === === === === ===
void BinaryStorage::replaceVault(
    const fs::path& tempPath,
    const fs::path& finalPath
) {
    try {
        fs::rename(tempPath, finalPath);
    } catch (const fs::filesystem_error& e) {
        std::error_code ec;
        fs::remove(tempPath, ec);

        throw StorageException(
            StorageCode::CannotRenameFile,
            std::string("Cannot rename file: ") + e.what()
        );
    }
}

void BinaryStorage::saveDraft(
    const VaultData& vaultData,
    const fs::path& tempPath
) {
    BinaryWriter writer;
    writer.writeVault(vaultData);

    SecureBuffer plaintext = writer.takeBuffer();

    EncryptResult encryptRes = Encryptor::encrypt(
        plaintext,
        SessionContext::get().getKey().data()
    );

    plaintext.clear();

    std::ofstream file(tempPath, std::ios::binary);
    if(!file.is_open()) {
        throw StorageException(
            StorageCode::CannotOpenFile,
            "Cannot open temporary vault file: " + tempPath.string()
        );
    }

    {
        BinaryWriter headerWriter;
        headerWriter.writeHeader();

        SecureBuffer header = headerWriter.takeBuffer();
        file.write(reinterpret_cast<char*>(header.data()), header.size());
    }

    file.write(reinterpret_cast<char*>(encryptRes.nonce.data()), encryptRes.nonce.size());
    file.write(reinterpret_cast<char*>(encryptRes.tag.data()), encryptRes.tag.size());
    file.write(reinterpret_cast<char*>(encryptRes.ciphertext.data()), encryptRes.ciphertext.size());

    if(!file) {
        throw StorageException(
            StorageCode::CannotWriteFile,
            "Failed to write vault file: " + tempPath.string()
        );
    }
}

// === === === === ===
// public:
// === === === === ===
void BinaryStorage::save(
    const VaultData& vaultData, 
    const fs::path& path
) {
    fs::path tempPath = path;
    tempPath += ".tmp";

    if(path.has_parent_path()) {
        fs::create_directory(path.parent_path());
    }

    saveDraft(vaultData, tempPath);
    replaceVault(tempPath, path);
}

VaultData BinaryStorage::load(const fs::path& path) {
    if (!fs::exists(path)) {
        throw StorageException(
            StorageCode::FileNotFound,
            "Vault file not found: " + path.string()
        );
    }

    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw StorageException(
            StorageCode::CannotOpenFile,
            "Cannot open vault file: " + path.string()
        );
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if(size == 0) {
        file.close();
        return VaultData{};
    }
    file.seekg(0, std::ios::beg);
    
    SecureBuffer buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    BinaryReader encryptReader(buffer);
    RawFile r = encryptReader.readHeader();

    SecureBuffer plaintext = Encryptor::decrypt(
        r.ciphertext,
        SessionContext::get().getKey().data(),
        r.nonce, r.tag
    );

    BinaryReader reader(plaintext);
    return reader.readVault();
}
